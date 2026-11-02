#include "Game/StageWorld.h"

namespace sclpr::game {

StageWorld::StageWorld() = default;

void StageWorld::Initialize(const sclpr::resource::LevelData& levelData,
    const glm::vec2& playerStart,
    sclpr::renderer::Camera2D* camera) {
    m_levelData = levelData;
    m_spawnPoint = playerStart;
    m_camera = camera;
    SpawnDefaultObjects();
}

void StageWorld::Update(float dt, PlayerController& player) {
    (void)dt;
    UpdateCameraFollow(player);

    for (auto& object : m_objects) {
        if (!object.active) {
            continue;
        }

        // Simple placeholder update loop for world objects.
        const float dx = object.position.x - player.GetPosition().x;
        if (std::abs(dx) < 32.0f) {
            object.active = false;
        }
    }
}

void StageWorld::Render() const {
    // Rendering will be handled by the scene / renderer in future work.
}

const sclpr::resource::LevelData& StageWorld::GetLevelData() const noexcept {
    return m_levelData;
}

const std::vector<ObjectSpawn>& StageWorld::GetObjects() const noexcept {
    return m_objects;
}

const glm::vec2& StageWorld::GetSpawnPoint() const noexcept {
    return m_spawnPoint;
}

void StageWorld::SpawnDefaultObjects() {
    m_objects.clear();
    for (int i = 0; i < 10; ++i) {
        ObjectSpawn object;
        object.id = "ring_" + std::to_string(i);
        object.type = "ring";
        object.position = glm::vec2(64.0f + i * 48.0f, 160.0f);
        object.variant = 0;
        object.active = true;
        m_objects.push_back(object);
    }
}

void StageWorld::UpdateCameraFollow(PlayerController& player) {
    if (!m_camera) {
        return;
    }

    glm::vec2 target = player.GetPosition();
    target.x = std::max(target.x, 0.0f);
    target.y = std::max(target.y, 0.0f);
    m_camera->SetPosition(target);
}

} // namespace sclpr::game
