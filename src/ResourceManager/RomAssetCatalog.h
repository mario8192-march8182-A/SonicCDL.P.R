#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace sclpr::resource {

struct AssetChunk {
    std::string name;
    std::string type;
    size_t offset{0};
    size_t size{0};
    std::vector<uint8_t> bytes;
};

class RomAssetCatalog {
public:
    RomAssetCatalog() = default;
    ~RomAssetCatalog() = default;

    // Extract a ROM asset catalog by scanning for known signature markers.
    // This is intentionally a second-step parser, as a more exact parse requires
    // knowledge of the internal layout of the Sonic CD ROM and the target region.
    static std::vector<AssetChunk> ScanForKnownChunks(const std::vector<uint8_t>& romData,
        const std::vector<std::string>& signatures);

    // Build a simple index of offsets for future managers: sprites, tiles, palettes, music.
    static std::vector<std::pair<std::string, size_t>> BuildAssetIndex(const std::vector<uint8_t>& romData,
        const std::vector<std::string>& signatures);
};

} // namespace sclpr::resource
