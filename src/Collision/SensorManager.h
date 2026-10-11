#pragma once

#include "Collision/FloorSensor.h"
#include "Collision/CeilingSensor.h"
#include "Collision/WallSensor.h"
#include <glm/glm.hpp>
#include <memory>

namespace sclpr::collision {

struct SensorData {
    bool grounded{false};
    bool hitCeiling{false};
    bool leftWallColliding{false};
    bool rightWallColliding{false};
    float floorDistance{0.0f};
    float ceilingDistance{0.0f};
    float leftWallDistance{0.0f};
    float rightWallDistance{0.0f};
    float floorAngle{0.0f};
    float ceilingAngle{0.0f};
};

class SensorManager {
public:
    SensorManager();
    ~SensorManager() = default;

    void Update(const glm::vec2& playerPos, float dt);

    FloorSensor& GetFloorSensor() noexcept;
    CeilingSensor& GetCeilingSensor() noexcept;
    WallSensor& GetLeftWallSensor() noexcept;
    WallSensor& GetRightWallSensor() noexcept;

    const FloorSensor& GetFloorSensor() const noexcept;
    const CeilingSensor& GetCeilingSensor() const noexcept;
    const WallSensor& GetLeftWallSensor() const noexcept;
    const WallSensor& GetRightWallSensor() const noexcept;

    const SensorData& GetSensorData() const noexcept;

    bool IsGrounded() const noexcept;
    bool HitCeiling() const noexcept;
    bool IsAgainstLeftWall() const noexcept;
    bool IsAgainstRightWall() const noexcept;

private:
    std::unique_ptr<FloorSensor> m_floorSensor;
    std::unique_ptr<CeilingSensor> m_ceilingSensor;
    std::unique_ptr<WallSensor> m_leftWallSensor;
    std::unique_ptr<WallSensor> m_rightWallSensor;
    SensorData m_sensorData;
};

} // namespace sclpr::collision
