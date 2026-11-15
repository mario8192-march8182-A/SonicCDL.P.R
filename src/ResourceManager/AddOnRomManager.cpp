#include "ResourceManager/AddOnRomManager.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iterator>

namespace sclpr::resource {
namespace {

std::string ToLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

std::string ReadRange(const std::vector<char>& bytes, size_t offset, size_t length) {
    if (offset >= bytes.size()) {
        return {};
    }
    const size_t count = std::min(length, bytes.size() - offset);
    return std::string(bytes.data() + offset, count);
}

bool Contains(const std::string& text, const std::string& needle) {
    return ToLower(text).find(ToLower(needle)) != std::string::npos;
}

} // namespace

bool AddOnRomManager::RegisterRom(const std::filesystem::path& romPath, AddOnRomInfo& outInfo) {
    AddOnRomInfo inspected = InspectRom(romPath);
    if (inspected.kind == AddOnRomKind::Unknown) {
        return false;
    }

    const auto existing = std::find_if(m_roms.begin(), m_roms.end(), [&](const AddOnRomInfo& item) {
        return item.path == inspected.path;
    });
    if (existing == m_roms.end()) {
        m_roms.push_back(inspected);
        outInfo = inspected;
    } else {
        *existing = inspected;
        outInfo = *existing;
    }
    return true;
}

bool AddOnRomManager::RemoveRom(const std::filesystem::path& romPath) {
    const auto oldSize = m_roms.size();
    std::erase_if(m_roms, [&](const AddOnRomInfo& item) {
        return item.path == romPath;
    });
    return m_roms.size() != oldSize;
}

const std::vector<AddOnRomInfo>& AddOnRomManager::GetRegisteredRoms() const noexcept {
    return m_roms;
}

const AddOnRomInfo* AddOnRomManager::Find(AddOnRomKind kind) const noexcept {
    const auto it = std::find_if(m_roms.begin(), m_roms.end(), [kind](const AddOnRomInfo& item) {
        return item.kind == kind;
    });
    return it == m_roms.end() ? nullptr : &*it;
}

AddOnRomInfo AddOnRomManager::InspectRom(const std::filesystem::path& romPath) {
    AddOnRomInfo info;
    std::error_code ec;
    if (!std::filesystem::is_regular_file(romPath, ec) || ec) {
        return info;
    }

    info.path = std::filesystem::weakly_canonical(romPath, ec);
    if (ec) {
        info.path = romPath.lexically_normal();
    }

    // Read only the small cartridge header region. This is identification, not
    // a ROM decoder or asset extractor. SMD-interleaved images are not decoded.
    std::ifstream input(romPath, std::ios::binary);
    if (!input) {
        return {};
    }
    std::array<char, 0x200> header{};
    input.read(header.data(), static_cast<std::streamsize>(header.size()));
    const auto bytesRead = input.gcount();
    if (bytesRead <= 0) {
        return {};
    }
    const std::vector<char> headerBytes(header.begin(), header.begin() + bytesRead);
    const std::string headerText = ReadRange(headerBytes, 0x100, 0x80);
    const std::string title = ReadRange(headerBytes, 0x120, 0x60);
    const std::string fileName = ToLower(romPath.filename().string());

    auto matches = [&](const char* titleToken, const char* filenameToken) {
        const bool titleMatch = Contains(title, titleToken);
        const bool filenameMatch = Contains(fileName, filenameToken);
        if (titleMatch) {
            info.headerMatched = true;
        }
        return titleMatch || filenameMatch;
    };

    if (matches("sonic and knuckles", "sonic3&knuckles") ||
        matches("sonic & knuckles", "sonic-and-knuckles") ||
        Contains(fileName, "sonicknuckles")) {
        info.kind = AddOnRomKind::Sonic3AndKnuckles;
        info.detectedTitle = "Sonic & Knuckles";
        info.providesPlayableCharacter = true;
        info.providesStageReference = true;
    } else if (matches("sonic the hedgehog 2", "sonic2") ||
               Contains(fileName, "sonic-the-hedgehog-2")) {
        info.kind = AddOnRomKind::Sonic2;
        info.detectedTitle = "Sonic the Hedgehog 2";
        info.providesPlayableCharacter = true;
        info.providesSpinDashReference = true;
    } else if (matches("sonic 3d blast", "sonic3dblast") ||
               Contains(fileName, "sonic-3d-blast")) {
        info.kind = AddOnRomKind::Sonic3DBlast;
        info.detectedTitle = "Sonic 3D Blast";
        info.providesStageReference = true;
        info.requires2DStageConversion = true;
    }

    // Avoid accepting unrelated files just because their name happens to
    // contain a game title unless the corresponding platform header is present.
    if (!info.headerMatched && headerText.size() >= 4 && headerText.find("SEGA") == std::string::npos) {
        info.kind = AddOnRomKind::Unknown;
        info.detectedTitle.clear();
    }

    return info;
}

} // namespace sclpr::resource
