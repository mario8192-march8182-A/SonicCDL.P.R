#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <memory>

namespace sclpr::resource {

struct LevelAsset {
    std::string id;
    std::string name;
    std::string build;
    std::string path;
    std::vector<uint8_t> bytes;
};

class LevelAssetLoader {
public:
    explicit LevelAssetLoader(std::string rootDirectory = "assets");
    ~LevelAssetLoader() = default;

    bool LoadLevelAsset(const std::string& relativePath,
        const std::string& build,
        LevelAsset& outAsset);

    std::vector<LevelAsset> LoadAllLevelAssets(const std::string& build) const;

private:
    std::string m_rootDirectory;
};

} // namespace sclpr::resource
