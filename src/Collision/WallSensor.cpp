#include "Collision/WallSensor.h"

#include <cmath>

namespace sclpr::collision {

WallSensor::WallSensor(WallSide side, glm::vec2 offset)
    : Sensor(SensorType::Wall, offset), m_wallSide(side) {}

bool WallSensor::CheckWall(const glm::vec2& worldPos, float& outDistance) noexcept {
    // Placeholder: In a real implementation, this would raycast left or right
    // and check against a tile map or collision geometry.
    // For now, we assume walls exist at x = -128 (left) and x = 2048 (right).

    if (m_wallSide == WallSide::Left) {
        const float leftWallX = -128.0f;
        const float distanceToWall = std::abs(worldPos.x - leftWallX);

        if (distanceToWall >= 0.0f && distanceToWall <= 16.0f) {
            outDistance = distanceToWall;
            return true;
        }
    } else {
        const float rightWallX = 2048.0f;
        const float distanceToWall = std::abs(worldPos.x - rightWallX);

        if (distanceToWall >= 0.0f && distanceToWall <= 16.0f) {
            outDistance = distanceToWall;
            return true;
        }
    }

    outDistance = -1.0f;
    return false;
}

WallSide WallSensor::GetWallSide() const noexcept {
    return m_wallSide;
}

bool WallSensor::IsWallPresent(float maxDistance) const noexcept {
    return IsColliding() && GetState().contactDistance <= maxDistance;
}

} // namespace sclpr::collision
