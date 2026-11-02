#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace sclpr::resource {

struct TileMapCell {
    int tileIndex{0};
    bool collision{false};
};

class TileMap {
public:
    TileMap() = default;
    ~TileMap() = default;

    void Resize(int width, int height);
    void SetTile(int x, int y, int tileIndex, bool collision = false);
    TileMapCell GetTile(int x, int y) const;

    int GetWidth() const noexcept;
    int GetHeight() const noexcept;

private:
    int m_width{0};
    int m_height{0};
    std::vector<TileMapCell> m_cells;
};

struct LevelData {
    std::string id;
    std::string name;
    std::string build;
    glm::vec2 spawnPoint{0.0f, 0.0f};
    TileMap tileMap;
};

class LevelDataIntegration {
public:
    static LevelData BuildLevelFromStage(const StageData& stage);
    static TileMap BuildTileMapFromStage(const StageData& stage);
};

} // namespace sclpr::resource
