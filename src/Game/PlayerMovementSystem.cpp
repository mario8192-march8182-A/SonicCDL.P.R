#include "Game/PlayerMovementSystem.h"

#include <algorithm>
#include <cmath>

namespace sclpr::game {

PlayerMovementSystem::PlayerMovementSystem()
    : m_physicsBody(std::make_unique<sclpr::physics::PhysicsBody>()),
      m_sensorManager(std::make_unique<sclpr::collision::SensorManager>()),
      m_collisionResolver(std::make_unique<sclpr::collision::CollisionResolver>()),
      m_stateMachine(std::make_unique<PlayerStateMachine>()),
      m_gemstoneController(std::make_unique<GemstoneSonicController>()) {}

void PlayerMovementSystem::Initialize(float startX, float startY) {
    m_physicsBody->SetPosition(startX, startY);
    m_physicsBody->SetVelocity(0.0f, 0.0f);
    m_physicsBody->SetGrounded(true);
    m_physicsBody->SetGravity(1600.0f);
    m_physicsBody->SetJumpStrength(540.0f);

    sclpr::physics::JumpPhysics::Reset(m_jumpState);
    sclpr::physics::WaterPhysics::Reset(m_waterState);
    sclpr::physics::SlopePhysics::Reset(m_slopeState);
    sclpr::physics::RollingPhysics::StopRolling(*m_physicsBody, m_rollingState);
    sclpr::physics::SpindashPhysics::Cancel(m_spindashState);
}

void PlayerMovementSystem::Update(const MovementInput& input, bool inWater, float dt) {
    m_currentInput = input;
    m_inWater = inWater;

    // 1. Update input state
    UpdateInput(input);

    // 2. Update physics (gravity, jump, water)
    UpdatePhysics(dt);

    // 3. Update sensors
    UpdateSensors(dt);

    // 4. Resolve collisions
    ResolveCollisions(dt);

    // 5. Apply slope physics
    ApplySlope(dt);

    // 6. Apply rolling
    ApplyRolling(dt);

    // 7. Apply spindash
    ApplySpindash(dt);

    // 8. Apply transformation effects
    ApplyTransformation(dt);

    // 9. Update movement states
    UpdateMovementStates();

    // 10. Update state machine
    UpdateStateMachine();
}

void PlayerMovementSystem::UpdateInput(const MovementInput& input) {
    m_currentInput.moveX = std::clamp(input.moveX, -1.0f, 1.0f);

    // Handle spindash
    if (input.spindashPressed && sclpr::physics::SpindashPhysics::CanSpindash(*m_physicsBody)) {
        sclpr::physics::SpindashPhysics::StartCharge(*m_physicsBody, m_spindashState);
    }

    // Handle rolling
    if (input.agatchPressed && sclpr::physics::RollingPhysics::CanRoll(*m_physicsBody)) {
        sclpr::physics::RollingPhysics::StartRolling(*m_physicsBody, m_rollingState, input.moveX);
    }

    // Handle transformation
    if (input.transformPressed) {
        m_gemstoneController->Activate();
    }
}

void PlayerMovementSystem::UpdatePhysics(float dt) {
    if (m_inWater) {
        sclpr::physics::WaterPhysics::Update(*m_physicsBody, m_waterState, m_currentInput.moveX, dt);
    } else {
        if (m_physicsBody->IsGrounded()) {
            sclpr::physics::GroundPhysics::Update(*m_physicsBody, m_currentInput.moveX, dt);
        } else {
            sclpr::physics::AirPhysics::Update(*m_physicsBody, dt);
        }
    }

    sclpr::physics::JumpPhysics::Update(*m_physicsBody, m_jumpState,
        m_currentInput.jumpPressed, m_currentInput.jumpHeld, dt);

    // Apply position change from velocity
    auto pos = m_physicsBody->GetPosition();
    auto vel = m_physicsBody->GetVelocity();
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
    m_physicsBody->SetPosition(pos);
}

void PlayerMovementSystem::UpdateSensors(float dt) {
    m_sensorManager->Update(m_physicsBody->GetPosition(), dt);
}

void PlayerMovementSystem::ResolveCollisions(float dt) {
    const auto& sensorData = m_sensorManager->GetSensorData();
    const auto resolved = m_collisionResolver->Resolve(
        m_physicsBody->GetPosition(),
        m_physicsBody->GetVelocity(),
        sensorData,
        dt
    );

    m_physicsBody->SetPosition(resolved.correctedPosition);
    m_physicsBody->SetVelocity(resolved.correctedVelocity.x, resolved.correctedVelocity.y);
    m_collisionResolver->UpdateGroundedState(*m_physicsBody, sensorData);
}

void PlayerMovementSystem::ApplySlope(float dt) {
    if (m_physicsBody->IsGrounded()) {
        const auto& sensorData = m_sensorManager->GetSensorData();
        m_slopeState.onSlope = true;
        m_slopeState.slopeAngle = sensorData.floorAngle;
        sclpr::physics::SlopePhysics::Update(*m_physicsBody, m_slopeState, m_currentInput.moveX, dt);
    } else {
        m_slopeState.onSlope = false;
    }
}

void PlayerMovementSystem::ApplyRolling(float dt) {
    if (m_rollingState.isRolling) {
        sclpr::physics::RollingPhysics::Update(*m_physicsBody, m_rollingState, m_currentInput.moveX, dt);
    }
}

void PlayerMovementSystem::ApplySpindash(float dt) {
    if (m_spindashState.isCharging || m_spindashState.isReleased) {
        sclpr::physics::SpindashPhysics::Update(*m_physicsBody, m_spindashState,
            m_currentInput.jumpHeld, dt);
    }
}

void PlayerMovementSystem::ApplyTransformation(float dt) {
    (void)dt;

    if (!m_gemstoneController->IsActive()) {
        return;
    }

    const float speedMultiplier = m_gemstoneController->GetSpec().speedMultiplier;
    const float jumpMultiplier = m_gemstoneController->GetSpec().jumpMultiplier;

    auto vel = m_physicsBody->GetVelocity();
    vel.x *= speedMultiplier;
    vel.y *= jumpMultiplier;
    m_physicsBody->SetVelocity(vel.x, vel.y);
}

void PlayerMovementSystem::UpdateMovementStates() {
    const auto& sensorData = m_sensorManager->GetSensorData();

    m_movementState.position = m_physicsBody->GetPosition();
    m_movementState.velocity = m_physicsBody->GetVelocity();
    m_movementState.grounded = sensorData.grounded;
    m_movementState.hitCeiling = sensorData.hitCeiling;
    m_movementState.leftWallColliding = sensorData.leftWallColliding;
    m_movementState.rightWallColliding = sensorData.rightWallColliding;
    m_movementState.inWater = m_inWater;
    m_movementState.onSlope = m_slopeState.onSlope;
    m_movementState.isRolling = m_rollingState.isRolling;
    m_movementState.isSpindashing = m_spindashState.isCharging || m_spindashState.isReleased;
    m_movementState.isTransformed = m_gemstoneController->IsActive();
    m_movementState.slopeAngle = m_slopeState.slopeAngle;
}

void PlayerMovementSystem::UpdateStateMachine() {
    m_stateMachine->Update(m_physicsBody->IsGrounded(), m_inWater,
        m_gemstoneController->IsActive(), 0.016f);
    m_movementState.playerState = m_stateMachine->GetState();
}

void PlayerMovementSystem::Render() const {
    // Renderer placeholder for sprite/animation integration.
}

const MovementState& PlayerMovementSystem::GetMovementState() const noexcept {
    return m_movementState;
}

const glm::vec2& PlayerMovementSystem::GetPosition() const noexcept {
    return m_movementState.position;
}

const glm::vec2& PlayerMovementSystem::GetVelocity() const noexcept {
    return m_movementState.velocity;
}

PlayerState PlayerMovementSystem::GetPlayerState() const noexcept {
    return m_movementState.playerState;
}

bool PlayerMovementSystem::IsGrounded() const noexcept {
    return m_movementState.grounded;
}

bool PlayerMovementSystem::IsRolling() const noexcept {
    return m_movementState.isRolling;
}

bool PlayerMovementSystem::IsSpindashing() const noexcept {
    return m_movementState.isSpindashing;
}

bool PlayerMovementSystem::IsTransformed() const noexcept {
    return m_movementState.isTransformed;
}

sclpr::physics::PhysicsBody& PlayerMovementSystem::GetPhysicsBody() noexcept {
    return *m_physicsBody;
}

sclpr::collision::SensorManager& PlayerMovementSystem::GetSensorManager() noexcept {
    return *m_sensorManager;
}

sclpr::physics::SlopeState& PlayerMovementSystem::GetSlopeState() noexcept {
    return m_slopeState;
}

sclpr::physics::RollingState& PlayerMovementSystem::GetRollingState() noexcept {
    return m_rollingState;
}

sclpr::physics::SpindashState& PlayerMovementSystem::GetSpindashState() noexcept {
    return m_spindashState;
}

} // namespace sclpr::game
