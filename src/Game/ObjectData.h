#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace sclpr::game {

enum class ObjectType {
    Ring,
    Enemy,
    Hazard,
    Checkpoint,
    Platform,
    Trigger,
    Unknown
};

struct ObjectData {
    std::string id;
    ObjectType type{ObjectType::Unknown};
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 size{16.0f, 16.0f};
    int variant{0};
    bool active{true};
    std::string build;
};

class ObjectSystem {
public:
    ObjectSystem() = default;
    ~ObjectSystem() = default;

    void AddObject(const ObjectData& object);
    void RemoveObject(const std::string& id);
    void Update(float dt, class StageWorld& world);

    const std::vector<ObjectData>& GetObjects() const noexcept;

private:
    std::vector<ObjectData> m_objects;
};

class EnemySystem {
public:
    EnemySystem() = default;
    ~EnemySystem() = default;

    void SpawnEnemy(const glm::vec2& position, int variant = 0, const std::string& build = "classic");
    void Update(float dt, class StageWorld& world);

    const std::vector<ObjectData>& GetEnemies() const noexcept;

private:
    std::vector<ObjectData> m_enemies;
};

class RingSystem {
public:
    RingSystem() = default;
    ~RingSystem() = default;

    void SpawnRing(const glm::vec2& position, int count = 1, const std::string& build = "classic");
    void Update(float dt, class StageWorld& world);

    const std::vector<ObjectData>& GetRings() const noexcept;

private:
    std::vector<ObjectData> m_rings;
};

} // namespace sclpr::game
