#include "ResourceManager/RomLoader.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace sclpr::resource {

RomLoader::RomLoader() = default;
RomLoader::~RomLoader() = default;

namespace {
std::string ToLower(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return input;
}

bool HasSignature(const std::string& text, const std::string& needle) {
    const std::string lowerText = ToLower(text);
    const std::string lowerNeedle = ToLower(needle);
    return lowerText.find(lowerNeedle) != std::string::npos;
}
} // namespace

std::vector<RomMetadata> RomLoader::DetectRoms() {
    std::vector<RomMetadata> detectedRoms;

    const std::vector<std::string> extensions = {".bin", ".iso", ".cue", ".img", ".ccd", ".md", ".gen", ".smd"};
    try {
        for (const auto& entry : fs::directory_iterator(fs::current_path())) {
            if (!entry.is_regular_file()) continue;

            const std::string ext = entry.path().extension().string();
            if (std::find(extensions.begin(), extensions.end(), ToLower(ext)) == extensions.end()) {
                continue;
            }

            RomMetadata metadata;
            if (ValidateRom(entry.path().string(), metadata)) {
                detectedRoms.push_back(metadata);
            }
        }
    } catch (...) {
    }

    return detectedRoms;
}

bool RomLoader::LoadRom(const std::string& filePath) {
    RomMetadata metadata;
    if (!ValidateRom(filePath, metadata)) {
        return false;
    }

    std::ifstream romFile(filePath, std::ios::binary);
    if (!romFile.is_open()) {
        return false;
    }

    romFile.seekg(0, std::ios::end);
    const std::streamoff size = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    if (size <= 0) {
        return false;
    }

    m_romData.resize(static_cast<size_t>(size));
    romFile.read(reinterpret_cast<char*>(m_romData.data()), static_cast<std::streamsize>(m_romData.size()));
    romFile.close();

    m_currentRom = metadata;
    m_romLoaded = true;
    BuildKnownResourceIndex();

    return true;
}

bool RomLoader::ValidateRom(const std::string& filePath, RomMetadata& outMetadata) {
    std::ifstream romFile(filePath, std::ios::binary);
    if (!romFile.is_open()) {
        return false;
    }

    romFile.seekg(0, std::ios::end);
    const std::streamoff size = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    if (size < 512 * 1024) {
        return false;
    }

    std::vector<uint8_t> header(256, 0);
    romFile.read(reinterpret_cast<char*>(header.data()), 256);
    romFile.close();

    if (!ParseRomHeader(header, outMetadata)) {
        return false;
    }

    outMetadata.filename = filePath;
    outMetadata.fileSize = static_cast<size_t>(size);
    outMetadata.isValid = true;
    return true;
}

bool RomLoader::ParseRomHeader(const std::vector<uint8_t>& headerData, RomMetadata& outMetadata) {
    if (headerData.size() < 256) {
        return false;
    }

    const std::string headerStr(reinterpret_cast<const char*>(headerData.data()), headerData.size());

    const bool hasSonicCD = HasSignature(headerStr, "Sonic CD") ||
                           HasSignature(headerStr, "SONIC CD") ||
                           HasSignature(headerStr, "sonic cd");
    const bool hasSega = HasSignature(headerStr, "SEGA");

    if (!hasSonicCD && !hasSega) {
        return false;
    }

    if (headerData.size() > 0xF0) {
        const uint8_t regionCode = headerData[0xF0];
        if (regionCode == 'J') {
            outMetadata.region = "JP";
            outMetadata.displayName = "Sonic CD (Japan)";
        } else if (regionCode == 'U') {
            outMetadata.region = "US";
            outMetadata.displayName = "Sonic CD (USA)";
        } else if (regionCode == 'E') {
            outMetadata.region = "EU";
            outMetadata.displayName = "Sonic CD (Europe)";
        } else {
            outMetadata.displayName = "Sonic CD";
        }
    } else {
        outMetadata.displayName = "Sonic CD";
    }

    return true;
}

void RomLoader::BuildKnownResourceIndex() {
    m_knownResources.clear();
    m_resourceOffsets.clear();

    if (m_romData.empty()) {
        return;
    }

    const std::vector<std::pair<std::string, std::string>> knownNames = {
        {"title", "text"},
        {"header", "header"},
        {"palette", "palette"},
        {"tilemap", "tilemap"},
        {"music", "audio"},
        {"sprite", "sprite"},
    };

    for (const auto& [name, type] : knownNames) {
        const std::string lowerRom(reinterpret_cast<const char*>(m_romData.data()),
                                    std::min<std::size_t>(m_romData.size(), 4096));
        const std::string lowerText = ToLower(lowerRom);

        const std::string needle = ToLower(name);
        const std::string::size_type pos = lowerText.find(needle);

        if (pos != std::string::npos) {
            m_resourceOffsets[name] = pos;
            m_knownResources.push_back({name, type, pos, std::min<std::size_t>(256, m_romData.size() - pos)});
        }
    }
}

const RomMetadata* RomLoader::GetLoadedRomMetadata() const noexcept {
    return m_romLoaded ? &m_currentRom : nullptr;
}

bool RomLoader::IsRomLoaded() const noexcept {
    return m_romLoaded;
}

std::vector<uint8_t> RomLoader::ExtractResource(const std::string& resourceName) {
    if (!m_romLoaded) {
        return {};
    }

    const auto it = m_resourceOffsets.find(resourceName);
    if (it == m_resourceOffsets.end()) {
        return {};
    }

    const size_t offset = it->second;
    if (offset >= m_romData.size()) {
        return {};
    }

    const size_t length = std::min<size_t>(256, m_romData.size() - offset);
    return std::vector<uint8_t>(m_romData.begin() + static_cast<std::ptrdiff_t>(offset),
                                m_romData.begin() + static_cast<std::ptrdiff_t>(offset + length));
}

std::vector<std::string> RomLoader::GetAvailableResources() const {
    std::vector<std::string> resources;
    for (const auto& pair : m_resourceOffsets) {
        resources.push_back(pair.first);
    }
    return resources;
}

std::vector<RomResource> RomLoader::GetKnownResources() const noexcept {
    return m_knownResources;
}

} // namespace sclpr::resource
