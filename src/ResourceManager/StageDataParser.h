#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

namespace sclpr::resource {

struct TileDefinition {
    int tileIndex{0};
    std::string tilesetName;
    glm::ivec2 size{16, 16};
    uint32_t flags{0};
};

struct TileMapLayer {
    std::string name;
    int width{0};
    int height{0};
    std::vector<int> tiles;
};

struct StageData {
    std::string id;
    std::string name;
    std::string build;
    int width{0};
    int height{0};
    std::vector<TileMapLayer> layers;
    std::vector<TileDefinition> tiles;
    glm::vec2 spawnPoint{0.0f, 0.0f};
};

class StageDataParser {
public:
    StageDataParser() = default;
    ~StageDataParser() = default;

    bool ParseStageData(const std::vector<uint8_t>& bytes, StageData& outStage);
    bool ParseStageLayer(const std::string& layerText, TileMapLayer& outLayer);
    bool ParseTileDefinition(const std::string& tileText, TileDefinition& outTile);
};

} // namespace sclpr::resource
