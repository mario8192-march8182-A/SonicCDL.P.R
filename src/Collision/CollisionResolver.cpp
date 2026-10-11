#include "Collision/CollisionResolver.h"

#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

namespace sclpr::collision {

CollisionResolution CollisionResolver::Resolve(const glm::vec2& currentPos,
    const glm::vec2& currentVel,
    const SensorData& sensorData,
    float dt) {
    (void)dt;

    CollisionResolution resolution;
    resolution.correctedPosition = currentPos;
    resolution.correctedVelocity = currentVel;

    // Resolve floor collision
    if (sensorData.grounded && sensorData.floorDistance >= 0.0f) {
        resolution.correctedPosition = ResolveFloor(currentPos, sensorData.floorDistance);
        resolution.positionCorrected = true;
    }

    // Resolve ceiling collision
    if (sensorData.hitCeiling && sensorData.ceilingDistance >= 0.0f) {
        resolution.correctedPosition = ResolveCeiling(resolution.correctedPosition, sensorData.ceilingDistance);
        resolution.correctedVelocity = ApplyCeilingCollision(resolution.correctedVelocity);
        resolution.positionCorrected = true;
    }

    // Resolve wall collision
    if ((sensorData.leftWallColliding || sensorData.rightWallColliding) &&
        (sensorData.leftWallDistance >= 0.0f || sensorData.rightWallDistance >= 0.0f)) {
        float wallDistance = std::max(sensorData.leftWallDistance, sensorData.rightWallDistance);
        resolution.correctedPosition = ResolveWall(resolution.correctedPosition,
            sensorData.leftWallColliding,
            sensorData.rightWallColliding,
            wallDistance);
        resolution.positionCorrected = true;
    }

    // Clamp velocity
    resolution.correctedVelocity = ClampVelocity(resolution.correctedVelocity, sensorData);
    resolution.velocityClamped = true;

    return resolution;
}

glm::vec2 CollisionResolver::ResolveFloor(const glm::vec2& currentPos,
    float floorDistance,
    float playerHeight) noexcept {
    glm::vec2 correctedPos = currentPos;
    // Push the player up by the distance to the floor.
    // This keeps the player's feet on or just above the floor.
    correctedPos.y -= floorDistance;
    return correctedPos;
}

glm::vec2 CollisionResolver::ResolveCeiling(const glm::vec2& currentPos,
    float ceilingDistance,
    float playerHeight) noexcept {
    glm::vec2 correctedPos = currentPos;
    // Push the player down by the distance to the ceiling.
    // This keeps the player's head below the ceiling.
    correctedPos.y += ceilingDistance;
    return correctedPos;
}

glm::vec2 CollisionResolver::ResolveWall(const glm::vec2& currentPos,
    bool leftWall,
    bool rightWall,
    float wallDistance,
    float playerWidth) noexcept {
    glm::vec2 correctedPos = currentPos;

    if (leftWall) {
        // Push the player right (away from the left wall).
        correctedPos.x += wallDistance + (playerWidth / 2.0f);
    } else if (rightWall) {
        // Push the player left (away from the right wall).
        correctedPos.x -= wallDistance + (playerWidth / 2.0f);
    }

    return correctedPos;
}

glm::vec2 CollisionResolver::ClampVelocity(const glm::vec2& velocity,
    const SensorData& sensorData) noexcept {
    glm::vec2 clampedVel = velocity;

    // If grounded, zero out downward velocity.
    if (sensorData.grounded && clampedVel.y > 0.0f) {
        clampedVel.y = 0.0f;
    }

    // If hitting ceiling, zero out upward velocity.
    if (sensorData.hitCeiling && clampedVel.y < 0.0f) {
        clampedVel.y = 0.0f;
    }

    // If hitting left wall and moving left, zero out leftward velocity.
    if (sensorData.leftWallColliding && clampedVel.x < 0.0f) {
        clampedVel.x = 0.0f;
    }

    // If hitting right wall and moving right, zero out rightward velocity.
    if (sensorData.rightWallColliding && clampedVel.x > 0.0f) {
        clampedVel.x = 0.0f;
    }

    return clampedVel;
}

void CollisionResolver::UpdateGroundedState(sclpr::physics::PhysicsBody& body,
    const SensorData& sensorData) noexcept {
    body.SetGrounded(sensorData.grounded);
}

glm::vec2 CollisionResolver::ApplyCeilingCollision(const glm::vec2& velocity) noexcept {
    glm::vec2 correctedVel = velocity;
    // Cut upward momentum when hitting the ceiling.
    if (correctedVel.y < 0.0f) {
        correctedVel.y = 0.0f;
    }
    return correctedVel;
}

} // namespace sclpr::collision
