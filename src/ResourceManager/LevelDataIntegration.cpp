#include "ResourceManager/LevelDataIntegration.h"

#include "ResourceManager/StageDataParser.h"

namespace sclpr::resource {

void TileMap::Resize(int width, int height) {
    m_width = width;
    m_height = height;
    m_cells.assign(static_cast<size_t>(width * height), TileMapCell{});
}

void TileMap::SetTile(int x, int y, int tileIndex, bool collision) {
    if (x < 0 || y < 0 || x >= m_width || y >= m_height) {
        return;
    }
    const size_t idx = static_cast<size_t>(y * m_width + x);
    if (idx >= m_cells.size()) {
        return;
    }
    m_cells[idx].tileIndex = tileIndex;
    m_cells[idx].collision = collision;
}

TileMapCell TileMap::GetTile(int x, int y) const {
    if (x < 0 || y < 0 || x >= m_width || y >= m_height) {
        return {};
    }
    const size_t idx = static_cast<size_t>(y * m_width + x);
    if (idx >= m_cells.size()) {
        return {};
    }
    return m_cells[idx];
}

int TileMap::GetWidth() const noexcept {
    return m_width;
}

int TileMap::GetHeight() const noexcept {
    return m_height;
}

LevelData LevelDataIntegration::BuildLevelFromStage(const StageData& stage) {
    LevelData level;
    level.id = stage.id;
    level.name = stage.name;
    level.build = stage.build;
    level.spawnPoint = stage.spawnPoint;
    level.tileMap = BuildTileMapFromStage(stage);
    return level;
}

TileMap LevelDataIntegration::BuildTileMapFromStage(const StageData& stage) {
    TileMap map;
    map.Resize(stage.width, stage.height);

    for (const auto& layer : stage.layers) {
        for (int y = 0; y < stage.height; ++y) {
            for (int x = 0; x < stage.width; ++x) {
                const int index = y * stage.width + x;
                if (index >= static_cast<int>(layer.tiles.size())) {
                    continue;
                }

                const int tileId = layer.tiles[index];
                map.SetTile(x, y, tileId, tileId != 0);
            }
        }
    }

    return map;
}

} // namespace sclpr::resource
