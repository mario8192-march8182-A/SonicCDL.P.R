#include "Game/ObjectData.h"

#include "Game/StageWorld.h"

namespace sclpr::game {

void ObjectSystem::AddObject(const ObjectData& object) {
    m_objects.push_back(object);
}

void ObjectSystem::RemoveObject(const std::string& id) {
    m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
        [&](const ObjectData& object) { return object.id == id; }), m_objects.end());
}

void ObjectSystem::Update(float dt, StageWorld& world) {
    (void)dt;
    (void)world;
}

const std::vector<ObjectData>& ObjectSystem::GetObjects() const noexcept {
    return m_objects;
}

void EnemySystem::SpawnEnemy(const glm::vec2& position, int variant, const std::string& build) {
    ObjectData enemy;
    enemy.id = "enemy_" + std::to_string(m_enemies.size());
    enemy.type = ObjectType::Enemy;
    enemy.position = position;
    enemy.size = glm::vec2(24.0f, 24.0f);
    enemy.variant = variant;
    enemy.active = true;
    enemy.build = build;
    m_enemies.push_back(enemy);
}

void EnemySystem::Update(float dt, StageWorld& world) {
    (void)dt;
    (void)world;
    for (auto& enemy : m_enemies) {
        if (!enemy.active) {
            continue;
        }
    }
}

const std::vector<ObjectData>& EnemySystem::GetEnemies() const noexcept {
    return m_enemies;
}

void RingSystem::SpawnRing(const glm::vec2& position, int count, const std::string& build) {
    for (int i = 0; i < count; ++i) {
        ObjectData ring;
        ring.id = "ring_" + std::to_string(m_rings.size());
        ring.type = ObjectType::Ring;
        ring.position = position + glm::vec2(static_cast<float>(i) * 12.0f, 0.0f);
        ring.size = glm::vec2(8.0f, 8.0f);
        ring.variant = 0;
        ring.active = true;
        ring.build = build;
        m_rings.push_back(ring);
    }
}

void RingSystem::Update(float dt, StageWorld& world) {
    (void)dt;
    (void)world;
    for (auto& ring : m_rings) {
        if (!ring.active) {
            continue;
        }
    }
}

const std::vector<ObjectData>& RingSystem::GetRings() const noexcept {
    return m_rings;
}

} // namespace sclpr::game
