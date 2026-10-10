#pragma once

#include "ResourceManager/RomLoader.h"

#include <memory>
#include <vector>

namespace sclpr::resource {

class ResourceManager {
public:
    explicit ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Initialize the resource manager
    void Initialize();

    // Detect available ROM files
    std::vector<RomMetadata> DetectAvailableRoms();

    // Load a ROM
    bool LoadRom(const std::string& filePath);

    // Check if ROM is loaded
    bool IsRomLoaded() const noexcept;

    // Get loaded ROM info
    const RomMetadata* GetRomInfo() const noexcept;

    // Get ROM loader for advanced operations
    RomLoader* GetRomLoader() noexcept { return m_romLoader.get(); }

private:
    std::unique_ptr<RomLoader> m_romLoader;
};

} // namespace sclpr::resource
