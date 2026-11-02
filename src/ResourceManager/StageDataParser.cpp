#include "ResourceManager/StageDataParser.h"

#include <sstream>
#include <string>
#include <vector>

namespace sclpr::resource {

bool StageDataParser::ParseStageData(const std::vector<uint8_t>& bytes, StageData& outStage) {
    if (bytes.empty()) {
        return false;
    }

    std::string text(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    std::stringstream ss(text);
    std::string line;
    bool inLayer = false;

    outStage.id = "stage_default";
    outStage.name = "Default Stage";
    outStage.build = "classic";
    outStage.width = 64;
    outStage.height = 32;
    outStage.spawnPoint = glm::vec2(64.0f, 128.0f);

    while (std::getline(ss, line)) {
        if (line.empty()) {
            continue;
        }

        if (line.rfind("[layer]", 0) == 0) {
            inLayer = true;
            TileMapLayer layer;
            layer.name = "layer";
            outStage.layers.push_back(layer);
            continue;
        }

        if (line.rfind("[tile]", 0) == 0) {
            TileDefinition tile;
            tile.tilesetName = "default";
            outStage.tiles.push_back(tile);
            continue;
        }

        if (inLayer && !outStage.layers.empty()) {
            outStage.layers.back().tiles.push_back(std::stoi(line));
        }
    }

    if (!outStage.layers.empty()) {
        outStage.layers.back().width = 64;
        outStage.layers.back().height = 32;
    }

    return true;
}

bool StageDataParser::ParseStageLayer(const std::string& layerText, TileMapLayer& outLayer) {
    std::stringstream ss(layerText);
    std::string value;
    while (std::getline(ss, value, ',')) {
        if (!value.empty()) {
            outLayer.tiles.push_back(std::stoi(value));
        }
    }
    return !outLayer.tiles.empty();
}

bool StageDataParser::ParseTileDefinition(const std::string& tileText, TileDefinition& outTile) {
    std::stringstream ss(tileText);
    std::string value;
    std::vector<std::string> items;
    while (std::getline(ss, value, ',')) {
        items.push_back(value);
    }

    if (items.size() >= 3) {
        outTile.tileIndex = std::stoi(items[0]);
        outTile.tilesetName = items[1];
        outTile.size.x = std::stoi(items[2]);
    }

    return true;
}

} // namespace sclpr::resource
