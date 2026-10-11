#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace sclpr::physics {

class PhysicsWorld {
public:
    struct CollisionEvent {
        glm::vec2 normal{0.0f, 0.0f};
        float penetration{0.0f};
    };

    void AddStaticBody(const glm::vec2& position, const glm::vec2& size);
    void Update(float dt);
    std::vector<CollisionEvent> GetResolvedCollisions() const;

private:
    std::vector<std::pair<glm::vec2, glm::vec2>> m_staticBodies;
    std::vector<CollisionEvent> m_collisionEvents;
};

} // namespace sclpr::physics
