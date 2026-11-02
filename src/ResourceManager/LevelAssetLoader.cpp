#include "ResourceManager/LevelAssetLoader.h"

#include <fstream>
#include <filesystem>

namespace sclpr::resource {

LevelAssetLoader::LevelAssetLoader(std::string rootDirectory)
    : m_rootDirectory(std::move(rootDirectory)) {}

bool LevelAssetLoader::LoadLevelAsset(const std::string& relativePath,
    const std::string& build,
    LevelAsset& outAsset) {
    const std::filesystem::path rootPath = std::filesystem::path(m_rootDirectory) / build;
    const std::filesystem::path fullPath = rootPath / relativePath;

    std::ifstream in(fullPath, std::ios::binary);
    if (!in.is_open()) {
        return false;
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    outAsset.id = relativePath;
    outAsset.name = relativePath;
    outAsset.build = build;
    outAsset.path = fullPath.string();
    outAsset.bytes = std::move(data);
    return true;
}

std::vector<LevelAsset> LevelAssetLoader::LoadAllLevelAssets(const std::string& build) const {
    std::vector<LevelAsset> assets;
    const std::filesystem::path rootPath = std::filesystem::path(m_rootDirectory) / build;

    if (!std::filesystem::exists(rootPath)) {
        return assets;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootPath)) {
        if (entry.is_regular_file()) {
            LevelAsset asset;
            asset.id = entry.path().filename().string();
            asset.name = entry.path().filename().string();
            asset.build = build;
            asset.path = entry.path().string();

            std::ifstream in(entry.path(), std::ios::binary);
            if (in.is_open()) {
                asset.bytes.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
                assets.push_back(asset);
            }
        }
    }

    return assets;
}

} // namespace sclpr::resource
