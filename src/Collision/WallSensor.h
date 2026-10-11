#pragma once

#include "Collision/Sensor.h"
#include <glm/glm.hpp>

namespace sclpr::collision {

enum class WallSide : std::uint8_t {
    Left,
    Right
};

struct WallSensorData {
    bool leftWallColliding{false};
    bool rightWallColliding{false};
    float leftWallDistance{0.0f};
    float rightWallDistance{0.0f};
    glm::vec2 leftContactPoint{0.0f, 0.0f};
    glm::vec2 rightContactPoint{0.0f, 0.0f};
};

class WallSensor : public Sensor {
public:
    explicit WallSensor(WallSide side, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    ~WallSensor() = default;

    // Check for wall collision at the sensor's world position.
    // Returns true if a wall is detected within maxDistance.
    bool CheckWall(const glm::vec2& worldPos, float& outDistance) noexcept;

    // Get which side this sensor monitors.
    WallSide GetWallSide() const noexcept;

    // Check if the wall is present and close enough.
    bool IsWallPresent(float maxDistance = 8.0f) const noexcept;

private:
    WallSide m_wallSide;
};

} // namespace sclpr::collision
