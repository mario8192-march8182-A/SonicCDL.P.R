#pragma once

#include "ResourceManager/LevelDataIntegration.h"
#include "Game/PlayerController.h"
#include "Renderer/Camera2D.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>

namespace sclpr::game {

struct ObjectSpawn {
    std::string id;
    std::string type;
    glm::vec2 position{0.0f, 0.0f};
    int variant{0};
    bool active{true};
};

class StageWorld {
public:
    StageWorld();
    ~StageWorld() = default;

    void Initialize(const sclpr::resource::LevelData& levelData,
        const glm::vec2& playerStart,
        sclpr::renderer::Camera2D* camera);

    void Update(float dt, PlayerController& player);
    void Render() const;

    const sclpr::resource::LevelData& GetLevelData() const noexcept;
    const std::vector<ObjectSpawn>& GetObjects() const noexcept;
    const glm::vec2& GetSpawnPoint() const noexcept;

private:
    void SpawnDefaultObjects();
    void UpdateCameraFollow(PlayerController& player);

    sclpr::resource::LevelData m_levelData;
    std::vector<ObjectSpawn> m_objects;
    glm::vec2 m_spawnPoint{0.0f, 0.0f};
    sclpr::renderer::Camera2D* m_camera{nullptr};
};

} // namespace sclpr::game
