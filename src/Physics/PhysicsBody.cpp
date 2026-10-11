#include "Physics/PhysicsBody.h"

#include <algorithm>

namespace sclpr::physics {

PhysicsBody::PhysicsBody() = default;

void PhysicsBody::SetPosition(float x, float y) noexcept {
    m_position.x = x;
    m_position.y = y;
}

void PhysicsBody::SetPosition(const glm::vec2& position) noexcept {
    m_position = position;
}

void PhysicsBody::SetVelocity(float x, float y) noexcept {
    m_velocity.x = x;
    m_velocity.y = y;
}

void PhysicsBody::SetVelocity(const glm::vec2& velocity) noexcept {
    m_velocity = velocity;
}

void PhysicsBody::SetGrounded(bool grounded) noexcept {
    m_grounded = grounded;
}

void PhysicsBody::SetGravity(float gravity) noexcept {
    m_gravity = gravity;
}

void PhysicsBody::SetGroundAcceleration(float acceleration) noexcept {
    m_groundAcceleration = acceleration;
}

void PhysicsBody::SetAirAcceleration(float acceleration) noexcept {
    m_airAcceleration = acceleration;
}

void PhysicsBody::SetJumpStrength(float jumpStrength) noexcept {
    m_jumpStrength = jumpStrength;
}

void PhysicsBody::SetMaxGroundSpeed(float maxSpeed) noexcept {
    m_maxGroundSpeed = maxSpeed;
}

void PhysicsBody::SetMaxAirSpeed(float maxSpeed) noexcept {
    m_maxAirSpeed = maxSpeed;
}

const glm::vec2& PhysicsBody::GetPosition() const noexcept {
    return m_position;
}

const glm::vec2& PhysicsBody::GetVelocity() const noexcept {
    return m_velocity;
}

bool PhysicsBody::IsGrounded() const noexcept {
    return m_grounded;
}

float PhysicsBody::GetGravity() const noexcept {
    return m_gravity;
}

float PhysicsBody::GetGroundAcceleration() const noexcept {
    return m_groundAcceleration;
}

float PhysicsBody::GetAirAcceleration() const noexcept {
    return m_airAcceleration;
}

float PhysicsBody::GetJumpStrength() const noexcept {
    return m_jumpStrength;
}

float PhysicsBody::GetMaxGroundSpeed() const noexcept {
    return m_maxGroundSpeed;
}

float PhysicsBody::GetMaxAirSpeed() const noexcept {
    return m_maxAirSpeed;
}

void PhysicsBody::MoveHorizontal(float input, float dt) {
    const float accel = m_grounded ? m_groundAcceleration : m_airAcceleration;
    const float maxSpeed = m_grounded ? m_maxGroundSpeed : m_maxAirSpeed;

    m_velocity.x += input * accel * dt;
    m_velocity.x = std::clamp(m_velocity.x, -maxSpeed, maxSpeed);

    if (m_grounded && std::abs(input) < 0.01f) {
        ApplyFriction(dt);
    }
}

void PhysicsBody::ApplyGravity(float dt) {
    if (!m_grounded) {
        m_velocity.y += m_gravity * dt;
    }
}

void PhysicsBody::Jump() {
    if (m_grounded) {
        m_velocity.y = -m_jumpStrength;
        m_grounded = false;
    }
}

void PhysicsBody::ApplyFriction(float dt) {
    const float friction = m_groundFriction * dt;
    if (std::abs(m_velocity.x) <= friction) {
        m_velocity.x = 0.0f;
    } else if (m_velocity.x > 0.0f) {
        m_velocity.x -= friction;
    } else {
        m_velocity.x += friction;
    }
}

void PhysicsBody::ClampVelocity() {
    const float maxSpeed = m_grounded ? m_maxGroundSpeed : m_maxAirSpeed;
    m_velocity.x = std::clamp(m_velocity.x, -maxSpeed, maxSpeed);
    if (!m_grounded) {
        m_velocity.y = std::clamp(m_velocity.y, -1500.0f, 1500.0f);
    }
}

} // namespace sclpr::physics
