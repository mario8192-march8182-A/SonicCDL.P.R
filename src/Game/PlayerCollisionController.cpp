#include "Game/PlayerCollisionController.h"

namespace sclpr::game {

PlayerCollisionController::PlayerCollisionController() = default;

void PlayerCollisionController::Initialize() {
    // Sensor manager is created in-place; future initialization can go here.
}

void PlayerCollisionController::Update(const glm::vec2& playerPosition, const glm::vec2& playerVelocity, float dt) {
    m_sensorManager.Update(playerPosition, dt);

    const auto& sensorData = m_sensorManager.GetSensorData();
    const auto resolved = m_collisionResolver.Resolve(playerPosition, playerVelocity, sensorData, dt);

    // The resolved state is captured here for later use by the player controller.
    (void)resolved;
}

const sclpr::collision::SensorManager& PlayerCollisionController::GetSensorManager() const noexcept {
    return m_sensorManager;
}

sclpr::collision::SensorManager& PlayerCollisionController::GetSensorManager() noexcept {
    return m_sensorManager;
}

const sclpr::collision::CollisionResolver& PlayerCollisionController::GetCollisionResolver() const noexcept {
    return m_collisionResolver;
}

sclpr::collision::CollisionResolver& PlayerCollisionController::GetCollisionResolver() noexcept {
    return m_collisionResolver;
}

bool PlayerCollisionController::IsGrounded() const noexcept {
    return m_sensorManager.IsGrounded();
}

bool PlayerCollisionController::HitCeiling() const noexcept {
    return m_sensorManager.HitCeiling();
}

bool PlayerCollisionController::IsAgainstLeftWall() const noexcept {
    return m_sensorManager.IsAgainstLeftWall();
}

bool PlayerCollisionController::IsAgainstRightWall() const noexcept {
    return m_sensorManager.IsAgainstRightWall();
}

} // namespace sclpr::game
