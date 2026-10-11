#pragma once

#include "Collision/SensorManager.h"
#include "Physics/PhysicsBody.h"
#include <glm/glm.hpp>

namespace sclpr::collision {

struct CollisionResolution {
    glm::vec2 correctedPosition{0.0f, 0.0f};
    glm::vec2 correctedVelocity{0.0f, 0.0f};
    bool velocityClamped{false};
    bool positionCorrected{false};
};

class CollisionResolver {
public:
    CollisionResolver() = default;
    ~CollisionResolver() = default;

    // Resolve collisions based on sensor data and current state.
    // Returns a CollisionResolution struct with corrected position and velocity.
    CollisionResolution Resolve(const glm::vec2& currentPos,
        const glm::vec2& currentVel,
        const SensorData& sensorData,
        float dt);

    // Resolve floor collision: push player above the floor.
    glm::vec2 ResolveFloor(const glm::vec2& currentPos,
        float floorDistance,
        float playerHeight = 16.0f) noexcept;

    // Resolve ceiling collision: push player below the ceiling.
    glm::vec2 ResolveCeiling(const glm::vec2& currentPos,
        float ceilingDistance,
        float playerHeight = 16.0f) noexcept;

    // Resolve wall collision: push player away from wall.
    glm::vec2 ResolveWall(const glm::vec2& currentPos,
        bool leftWall,
        bool rightWall,
        float wallDistance,
        float playerWidth = 8.0f) noexcept;

    // Clamp velocity based on collision state.
    glm::vec2 ClampVelocity(const glm::vec2& velocity,
        const SensorData& sensorData) noexcept;

    // Set grounded state based on floor contact.
    void UpdateGroundedState(sclpr::physics::PhysicsBody& body,
        const SensorData& sensorData) noexcept;

    // Apply ceiling collision to velocity (cut upward momentum).
    glm::vec2 ApplyCeilingCollision(const glm::vec2& velocity) noexcept;

private:
    float m_playerHeight{16.0f};
    float m_playerWidth{8.0f};
};

} // namespace sclpr::collision
