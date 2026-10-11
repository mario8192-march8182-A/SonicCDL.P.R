#pragma once

#include "Collision/SensorManager.h"
#include "Collision/CollisionResolver.h"
#include "Physics/PhysicsBody.h"

#include <glm/glm.hpp>

namespace sclpr::game {

class PlayerController;

class PlayerCollisionController {
public:
    PlayerCollisionController();
    ~PlayerCollisionController() = default;

    void Initialize();
    void Update(const glm::vec2& playerPosition, const glm::vec2& playerVelocity, float dt);

    const sclpr::collision::SensorManager& GetSensorManager() const noexcept;
    sclpr::collision::SensorManager& GetSensorManager() noexcept;
    const sclpr::collision::CollisionResolver& GetCollisionResolver() const noexcept;
    sclpr::collision::CollisionResolver& GetCollisionResolver() noexcept;

    bool IsGrounded() const noexcept;
    bool HitCeiling() const noexcept;
    bool IsAgainstLeftWall() const noexcept;
    bool IsAgainstRightWall() const noexcept;

private:
    sclpr::collision::SensorManager m_sensorManager;
    sclpr::collision::CollisionResolver m_collisionResolver;
};

} // namespace sclpr::game
