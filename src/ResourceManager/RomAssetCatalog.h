#pragma once

#include <string>
#include <vector>
#include <array>
#include <cstdint>

namespace sclpr::resource {

enum class AssetKind {
    Unknown,
    Palette,
    Sprite,
    Tile,
    Music,
    Sound,
    Level,
    Text,
    Binary
};

struct AssetChunk {
    std::string name;
    std::string type;
    std::string platform;
    AssetKind kind{AssetKind::Unknown};
    size_t offset{0};
    size_t size{0};
    std::vector<uint8_t> bytes;
};

class RomAssetCatalog {
public:
    RomAssetCatalog() = default;
    ~RomAssetCatalog() = default;

    static std::vector<AssetChunk> ScanForKnownChunks(const std::vector<uint8_t>& romData,
        const std::vector<std::string>& signatures,
        const std::string& platform = "generic");

    static std::vector<std::pair<std::string, size_t>> BuildAssetIndex(const std::vector<uint8_t>& romData,
        const std::vector<std::string>& signatures,
        const std::string& platform = "generic");

    static std::vector<AssetChunk> BuildCatalogForBuild(const std::vector<uint8_t>& romData,
        const std::string& platform = "generic");

    static AssetKind ClassifyByName(const std::string& assetName);
};

} // namespace sclpr::resource
