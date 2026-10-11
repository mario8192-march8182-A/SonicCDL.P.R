#include "Game/PlayerController.h"

#include <algorithm>
#include <cmath>

namespace sclpr::game {

PlayerController::PlayerController() = default;

void PlayerController::Initialize(float startX, float startY) {
    m_physicsBody.SetPosition(startX, startY);
    m_physicsBody.SetVelocity(0.0f, 0.0f);
    m_physicsBody.SetGrounded(true);
    m_physicsBody.SetGravity(1600.0f);
    m_physicsBody.SetJumpStrength(540.0f);

    sclpr::physics::JumpPhysics::Reset(m_jumpState);
    sclpr::physics::WaterPhysics::Reset(m_waterState);
}

void PlayerController::Update(float moveInput, bool jumpPressed, bool jumpHeld, bool inWater, float dt) {
    m_inputX = std::clamp(moveInput, -1.0f, 1.0f);

    UpdateMovement(m_inputX, jumpPressed, jumpHeld, inWater, dt);
    m_stateMachine.Update(m_physicsBody.IsGrounded(), inWater, m_gemstoneController.IsActive(), dt);
    ApplyTransformationModifiers(dt);
}

void PlayerController::UpdateMovement(float moveInput, bool jumpPressed, bool jumpHeld, bool inWater, float dt) {
    m_waterState.inWater = inWater;

    if (inWater) {
        sclpr::physics::WaterPhysics::Update(m_physicsBody, m_waterState, moveInput, dt);
    } else {
        if (m_physicsBody.IsGrounded()) {
            sclpr::physics::GroundPhysics::Update(m_physicsBody, moveInput, dt);
        } else {
            sclpr::physics::AirPhysics::Update(m_physicsBody, dt);
        }
    }

    sclpr::physics::JumpPhysics::Update(m_physicsBody, m_jumpState, jumpPressed, jumpHeld, dt);

    auto pos = m_physicsBody.GetPosition();
    auto vel = m_physicsBody.GetVelocity();
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
    m_physicsBody.SetPosition(pos);
}

void PlayerController::ApplyTransformationModifiers(float dt) {
    (void)dt;

    if (!m_gemstoneController.IsActive()) {
        return;
    }

    const float speedMultiplier = m_gemstoneController.GetSpec().speedMultiplier;
    const float jumpMultiplier = m_gemstoneController.GetSpec().jumpMultiplier;

    auto vel = m_physicsBody.GetVelocity();
    vel.x *= speedMultiplier;
    vel.y *= jumpMultiplier;
    m_physicsBody.SetVelocity(vel.x, vel.y);

    if (m_physicsBody.GetJumpStrength() < 999.0f) {
        m_physicsBody.SetJumpStrength(m_jumpStrength * jumpMultiplier);
    }
}

void PlayerController::Render() const {
    // Renderer placeholder for sprite/animation integration.
}

void PlayerController::SetTransformation(bool enabled) {
    if (enabled) {
        m_gemstoneController.Activate();
    } else {
        m_gemstoneController.Deactivate();
    }
}

void PlayerController::SetGemstoneSonic(bool enabled) {
    SetTransformation(enabled);
}

const glm::vec2& PlayerController::GetPosition() const noexcept {
    return m_physicsBody.GetPosition();
}

const glm::vec2& PlayerController::GetVelocity() const noexcept {
    return m_physicsBody.GetVelocity();
}

PlayerState PlayerController::GetState() const noexcept {
    return m_stateMachine.GetState();
}

bool PlayerController::IsGrounded() const noexcept {
    return m_physicsBody.IsGrounded();
}

bool PlayerController::IsInWater() const noexcept {
    return m_waterState.inWater;
}

bool PlayerController::IsTransformed() const noexcept {
    return m_gemstoneController.IsActive();
}

sclpr::physics::PhysicsBody& PlayerController::GetPhysicsBody() noexcept {
    return m_physicsBody;
}

const sclpr::physics::PhysicsBody& PlayerController::GetPhysicsBody() const noexcept {
    return m_physicsBody;
}

} // namespace sclpr::game
