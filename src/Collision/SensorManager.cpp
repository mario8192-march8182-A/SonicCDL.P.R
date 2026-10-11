#include "Collision/SensorManager.h"

namespace sclpr::collision {

SensorManager::SensorManager()
    : m_floorSensor(std::make_unique<FloorSensor>()),
      m_ceilingSensor(std::make_unique<CeilingSensor>()),
      m_leftWallSensor(std::make_unique<WallSensor>(WallSide::Left, glm::vec2(-8.0f, 0.0f))),
      m_rightWallSensor(std::make_unique<WallSensor>(WallSide::Right, glm::vec2(8.0f, 0.0f))) {}

void SensorManager::Update(const glm::vec2& playerPos, float dt) {
    (void)dt;

    m_floorSensor->SetWorldPosition(playerPos);
    m_ceilingSensor->SetWorldPosition(playerPos);
    m_leftWallSensor->SetWorldPosition(playerPos);
    m_rightWallSensor->SetWorldPosition(playerPos);

    // Check floor
    float floorDistance = 0.0f;
    if (m_floorSensor->CheckFloor(playerPos, floorDistance)) {
        SensorResult floorResult;
        floorResult.colliding = true;
        floorResult.contactDistance = floorDistance;
        floorResult.contactAngle = 0.0f;
        m_floorSensor->RegisterCollision(floorResult);
        m_sensorData.grounded = true;
        m_sensorData.floorDistance = floorDistance;
        m_sensorData.floorAngle = 0.0f;
    } else {
        m_floorSensor->ClearCollision();
        m_sensorData.grounded = false;
        m_sensorData.floorDistance = -1.0f;
    }

    // Check ceiling
    float ceilingDistance = 0.0f;
    if (m_ceilingSensor->CheckCeiling(playerPos, ceilingDistance)) {
        SensorResult ceilingResult;
        ceilingResult.colliding = true;
        ceilingResult.contactDistance = ceilingDistance;
        ceilingResult.contactAngle = 0.0f;
        m_ceilingSensor->RegisterCollision(ceilingResult);
        m_sensorData.hitCeiling = true;
        m_sensorData.ceilingDistance = ceilingDistance;
        m_sensorData.ceilingAngle = 0.0f;
    } else {
        m_ceilingSensor->ClearCollision();
        m_sensorData.hitCeiling = false;
        m_sensorData.ceilingDistance = -1.0f;
    }

    // Check left wall
    float leftWallDistance = 0.0f;
    if (m_leftWallSensor->CheckWall(playerPos, leftWallDistance)) {
        SensorResult leftWallResult;
        leftWallResult.colliding = true;
        leftWallResult.contactDistance = leftWallDistance;
        leftWallResult.contactAngle = 0.0f;
        m_leftWallSensor->RegisterCollision(leftWallResult);
        m_sensorData.leftWallColliding = true;
        m_sensorData.leftWallDistance = leftWallDistance;
    } else {
        m_leftWallSensor->ClearCollision();
        m_sensorData.leftWallColliding = false;
        m_sensorData.leftWallDistance = -1.0f;
    }

    // Check right wall
    float rightWallDistance = 0.0f;
    if (m_rightWallSensor->CheckWall(playerPos, rightWallDistance)) {
        SensorResult rightWallResult;
        rightWallResult.colliding = true;
        rightWallResult.contactDistance = rightWallDistance;
        rightWallResult.contactAngle = 0.0f;
        m_rightWallSensor->RegisterCollision(rightWallResult);
        m_sensorData.rightWallColliding = true;
        m_sensorData.rightWallDistance = rightWallDistance;
    } else {
        m_rightWallSensor->ClearCollision();
        m_sensorData.rightWallColliding = false;
        m_sensorData.rightWallDistance = -1.0f;
    }
}

FloorSensor& SensorManager::GetFloorSensor() noexcept {
    return *m_floorSensor;
}

CeilingSensor& SensorManager::GetCeilingSensor() noexcept {
    return *m_ceilingSensor;
}

WallSensor& SensorManager::GetLeftWallSensor() noexcept {
    return *m_leftWallSensor;
}

WallSensor& SensorManager::GetRightWallSensor() noexcept {
    return *m_rightWallSensor;
}

const FloorSensor& SensorManager::GetFloorSensor() const noexcept {
    return *m_floorSensor;
}

const CeilingSensor& SensorManager::GetCeilingSensor() const noexcept {
    return *m_ceilingSensor;
}

const WallSensor& SensorManager::GetLeftWallSensor() const noexcept {
    return *m_leftWallSensor;
}

const WallSensor& SensorManager::GetRightWallSensor() const noexcept {
    return *m_rightWallSensor;
}

const SensorData& SensorManager::GetSensorData() const noexcept {
    return m_sensorData;
}

bool SensorManager::IsGrounded() const noexcept {
    return m_sensorData.grounded;
}

bool SensorManager::HitCeiling() const noexcept {
    return m_sensorData.hitCeiling;
}

bool SensorManager::IsAgainstLeftWall() const noexcept {
    return m_sensorData.leftWallColliding;
}

bool SensorManager::IsAgainstRightWall() const noexcept {
    return m_sensorData.rightWallColliding;
}

} // namespace sclpr::collision
