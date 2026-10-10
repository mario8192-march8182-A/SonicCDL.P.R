#include "ResourceManager/RomLoader.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

namespace sclpr::resource {

RomLoader::RomLoader() = default;

RomLoader::~RomLoader() = default;

std::vector<RomMetadata> RomLoader::DetectRoms() {
    std::vector<RomMetadata> detectedRoms;

    // Standard ROM file extensions
    const std::vector<std::string> extensions = {
        ".bin", ".iso", ".cue", ".img", ".ccd",
        ".md", ".gen", ".smd"
    };

    // Search in current directory
    try {
        for (const auto& entry : fs::directory_iterator(fs::current_path())) {
            if (!entry.is_regular_file()) continue;

            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            // Check if file has a ROM extension
            if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
                RomMetadata metadata;
                if (ValidateRom(entry.path().string(), metadata)) {
                    detectedRoms.push_back(metadata);
                }
            }
        }
    } catch (const std::exception& e) {
        // Silently continue if directory scan fails
    }

    // Also check common ROM directories
    std::vector<std::string> commonDirs = {
        "ROMs",
        "Games",
        "Sonic CD",
        "SonicCD"
    };

    for (const auto& dir : commonDirs) {
        if (!fs::exists(dir)) continue;
        try {
            for (const auto& entry : fs::directory_iterator(dir)) {
                if (!entry.is_regular_file()) continue;

                std::string ext = entry.path().extension().string();
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
                    RomMetadata metadata;
                    if (ValidateRom(entry.path().string(), metadata)) {
                        // Avoid duplicates
                        bool isDuplicate = false;
                        for (const auto& detected : detectedRoms) {
                            if (detected.fileSize == metadata.fileSize &&
                                detected.displayName == metadata.displayName) {
                                isDuplicate = true;
                                break;
                            }
                        }
                        if (!isDuplicate) {
                            detectedRoms.push_back(metadata);
                        }
                    }
                }
            }
        } catch (const std::exception& e) {
            // Silently continue
        }
    }

    return detectedRoms;
}

bool RomLoader::LoadRom(const std::string& filePath) {
    RomMetadata metadata;

    if (!ValidateRom(filePath, metadata)) {
        return false;
    }

    // Read the ROM file into memory
    std::ifstream romFile(filePath, std::ios::binary);
    if (!romFile.is_open()) {
        return false;
    }

    // Get file size
    romFile.seekg(0, std::ios::end);
    size_t fileSize = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    // Read entire file
    m_romData.resize(fileSize);
    romFile.read(reinterpret_cast<char*>(m_romData.data()), fileSize);
    romFile.close();

    m_currentRom = metadata;
    m_romLoaded = true;

    // Build resource offset map (simplified for now)
    // Full implementation would parse the actual ROM structure
    m_resourceOffsets.clear();

    return true;
}

bool RomLoader::ValidateRom(const std::string& filePath, RomMetadata& outMetadata) {
    std::ifstream romFile(filePath, std::ios::binary);
    if (!romFile.is_open()) {
        return false;
    }

    // Check file size
    romFile.seekg(0, std::ios::end);
    size_t fileSize = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    // Sonic CD ROM is typically 512 KB or larger
    if (fileSize < 512 * 1024) {
        return false;
    }

    // Read header (first 256 bytes)
    std::vector<uint8_t> header(256);
    romFile.read(reinterpret_cast<char*>(header.data()), 256);
    romFile.close();

    // Validate using ROM header
    if (!ParseRomHeader(header, outMetadata)) {
        return false;
    }

    outMetadata.filename = filePath;
    outMetadata.fileSize = fileSize;
    outMetadata.isValid = true;

    return true;
}

bool RomLoader::ParseRomHeader(const std::vector<uint8_t>& headerData, RomMetadata& outMetadata) {
    // Sonic CD ROM header validation
    // Offset 0x100-0x10F: System type (should be "SEGA" for Genesis/Mega Drive)
    // Offset 0x120-0x14F: Copyright/Title info

    if (headerData.size() < 256) {
        return false;
    }

    // Check for Sonic CD signature in header
    // "Sonic" typically appears in the ROM header
    std::string headerStr(reinterpret_cast<const char*>(headerData.data()), headerData.size());

    bool hasSonicSignature = (headerStr.find("Sonic CD") != std::string::npos ||
                             headerStr.find("SONIC CD") != std::string::npos ||
                             headerStr.find("sonic cd") != std::string::npos);

    // Also check for general Sega Genesis/Mega Drive signature
    bool hasSegaSignature = (headerStr.find("SEGA") != std::string::npos);

    if (!hasSonicSignature && !hasSegaSignature) {
        return false;
    }

    // Determine region from header
    // Typically at offset 0xF0 in Genesis ROM
    if (headerData.size() > 0xF0) {
        uint8_t regionCode = headerData[0xF0];
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

    auto it = m_resourceOffsets.find(resourceName);
    if (it == m_resourceOffsets.end()) {
        return {};
    }

    // Simplified extraction - full implementation would parse specific ROM structures
    size_t offset = it->second;
    if (offset >= m_romData.size()) {
        return {};
    }

    return m_romData;
}

std::vector<std::string> RomLoader::GetAvailableResources() const {
    std::vector<std::string> resources;
    for (const auto& pair : m_resourceOffsets) {
        resources.push_back(pair.first);
    }
    return resources;
}

} // namespace sclpr::resource
