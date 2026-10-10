#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace sclpr::resource {

struct RomMetadata {
    std::string filename;
    std::string displayName;
    bool isValid{false};
    size_t fileSize{0};
    std::string region;  // "US", "JP", "EU"
};

class RomLoader {
public:
    explicit RomLoader();
    ~RomLoader();

    RomLoader(const RomLoader&) = delete;
    RomLoader& operator=(const RomLoader&) = delete;

    // Detect and validate ROM files in the current directory and standard locations
    std::vector<RomMetadata> DetectRoms();

    // Load a specific ROM/disk image
    bool LoadRom(const std::string& filePath);

    // Get the current loaded ROM metadata
    const RomMetadata* GetLoadedRomMetadata() const noexcept;

    // Check if a ROM is currently loaded
    bool IsRomLoaded() const noexcept;

    // Extract a file from the ROM by name
    std::vector<uint8_t> ExtractResource(const std::string& resourceName);

    // Get list of available resources in the loaded ROM
    std::vector<std::string> GetAvailableResources() const;

private:
    bool ValidateRom(const std::string& filePath, RomMetadata& outMetadata);
    bool ParseRomHeader(const std::vector<uint8_t>& headerData, RomMetadata& outMetadata);

    RomMetadata m_currentRom;
    std::vector<uint8_t> m_romData;
    std::unordered_map<std::string, size_t> m_resourceOffsets;  // Resource name -> offset in ROM
    bool m_romLoaded{false};
};

} // namespace sclpr::resource
