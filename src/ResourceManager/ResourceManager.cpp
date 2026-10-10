#include "ResourceManager/ResourceManager.h"

namespace sclpr::resource {

ResourceManager::ResourceManager() {
    m_romLoader = std::make_unique<RomLoader>();
}

ResourceManager::~ResourceManager() = default;

void ResourceManager::Initialize() {
    // Initialize resource systems
}

std::vector<RomMetadata> ResourceManager::DetectAvailableRoms() {
    if (m_romLoader) {
        return m_romLoader->DetectRoms();
    }
    return {};
}

bool ResourceManager::LoadRom(const std::string& filePath) {
    if (m_romLoader) {
        return m_romLoader->LoadRom(filePath);
    }
    return false;
}

bool ResourceManager::IsRomLoaded() const noexcept {
    if (m_romLoader) {
        return m_romLoader->IsRomLoaded();
    }
    return false;
}

const RomMetadata* ResourceManager::GetRomInfo() const noexcept {
    if (m_romLoader) {
        return m_romLoader->GetLoadedRomMetadata();
    }
    return nullptr;
}

} // namespace sclpr::resource
