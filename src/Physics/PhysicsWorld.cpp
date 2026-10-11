#include "Physics/PhysicsWorld.h"

namespace sclpr::physics {

void PhysicsWorld::AddStaticBody(const glm::vec2& position, const glm::vec2& size) {
    m_staticBodies.emplace_back(position, size);
}

void PhysicsWorld::Update(float dt) {
    m_collisionEvents.clear();
    (void)dt;
}

std::vector<PhysicsWorld::CollisionEvent> PhysicsWorld::GetResolvedCollisions() const {
    return m_collisionEvents;
}

} // namespace sclpr::physics
