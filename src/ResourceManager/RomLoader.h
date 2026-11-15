#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace sclpr::resource {

struct RomResource {
    std::string name;
    std::string type;
    size_t offset{0};
    size_t size{0};
};

struct RomMetadata {
    std::string filename;
    std::string displayName;
    bool isValid{false};
    size_t fileSize{0};
    std::string region;
};

class RomLoader {
public:
    explicit RomLoader();
    ~RomLoader();

    RomLoader(const RomLoader&) = delete;
    RomLoader& operator=(const RomLoader&) = delete;

    std::vector<RomMetadata> DetectRoms();
    bool LoadRom(const std::string& filePath);
    const RomMetadata* GetLoadedRomMetadata() const noexcept;
    bool IsRomLoaded() const noexcept;

    std::vector<uint8_t> ExtractResource(const std::string& resourceName);
    std::vector<std::string> GetAvailableResources() const;
    std::vector<RomResource> GetKnownResources() const noexcept;

private:
    bool ValidateRom(const std::string& filePath, RomMetadata& outMetadata);
    bool ParseRomHeader(const std::vector<uint8_t>& headerData, RomMetadata& outMetadata);
    void BuildKnownResourceIndex();

    RomMetadata m_currentRom;
    std::vector<uint8_t> m_romData;
    std::unordered_map<std::string, size_t> m_resourceOffsets;
    std::vector<RomResource> m_knownResources;
    bool m_romLoaded{false};
};

} // namespace sclpr::resource
