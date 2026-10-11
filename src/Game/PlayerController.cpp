#include "Game/PlayerController.h"

#include <cmath>
#include <algorithm>

namespace sclpr::game {

PlayerController::PlayerController() = default;

void PlayerController::Initialize(float startX, float startY) {
    m_physicsBody.SetPosition(startX, startY);
    m_physicsBody.SetVelocity(0.0f, 0.0f);
    m_physicsBody.SetGrounded(true);
    m_state = PlayerState::Idle;
    sclpr::physics::JumpPhysics::Reset(m_jumpState);
    sclpr::physics::WaterPhysics::Reset(m_waterState);
}

void PlayerController::Update(float moveInput, bool jumpPressed, bool jumpHeld, bool inWater, float dt) {
    m_inputX = std::clamp(moveInput, -1.0f, 1.0f);
    m_waterState.inWater = inWater;

    if (inWater) {
        sclpr::physics::WaterPhysics::Update(m_physicsBody, m_waterState, m_inputX, dt);
    } else {
        if (m_physicsBody.IsGrounded()) {
            sclpr::physics::GroundPhysics::Update(m_physicsBody, m_inputX, dt);
        } else {
            sclpr::physics::AirPhysics::Update(m_physicsBody, dt);
        }
    }

    sclpr::physics::JumpPhysics::Update(m_physicsBody, m_jumpState, jumpPressed, jumpHeld, dt);
    UpdateState(m_inputX, jumpPressed, dt);
    ApplyMovement(dt);
}

void PlayerController::UpdateState(float moveInput, bool jumpPressed, float dt) {
    (void)dt;

    if (m_waterState.inWater) {
        m_state = PlayerState::InWater;
        return;
    }

    if (!m_physicsBody.IsGrounded()) {
        if (m_physicsBody.GetVelocity().y < 0.0f) {
            m_state = PlayerState::Jumping;
        } else {
            m_state = PlayerState::Falling;
        }
        return;
    }

    if (std::abs(moveInput) < 0.01f) {
        m_state = PlayerState::Idle;
    } else if (std::abs(m_physicsBody.GetVelocity().x) < 200.0f) {
        m_state = PlayerState::Walking;
    } else {
        m_state = PlayerState::Running;
    }
}

void PlayerController::ApplyMovement(float dt) {
    auto pos = m_physicsBody.GetPosition();
    auto vel = m_physicsBody.GetVelocity();

    pos.x += vel.x * dt;
    pos.y += vel.y * dt;

    m_physicsBody.SetPosition(pos);
}

void PlayerController::Render() const {
    // Placeholder: will be replaced with sprite rendering
}

const glm::vec2& PlayerController::GetPosition() const noexcept {
    return m_physicsBody.GetPosition();
}

const glm::vec2& PlayerController::GetVelocity() const noexcept {
    return m_physicsBody.GetVelocity();
}

PlayerState PlayerController::GetState() const noexcept {
    return m_state;
}

bool PlayerController::IsGrounded() const noexcept {
    return m_physicsBody.IsGrounded();
}

bool PlayerController::IsInWater() const noexcept {
    return m_waterState.inWater;
}

sclpr::physics::PhysicsBody& PlayerController::GetPhysicsBody() noexcept {
    return m_physicsBody;
}

const sclpr::physics::PhysicsBody& PlayerController::GetPhysicsBody() const noexcept {
    return m_physicsBody;
}

} // namespace sclpr::game
