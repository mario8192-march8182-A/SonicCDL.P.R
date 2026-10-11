#pragma once

#include "Game/PlayerController.h"
#include "Game/PlayerCollisionController.h"
#include "Game/PlayerStateMachine.h"
#include "Game/GemstoneSonicController.h"
#include "Physics/PhysicsBody.h"
#include "Physics/JumpPhysics.h"
#include "Physics/WaterPhysics.h"
#include "Physics/GroundPhysics.h"
#include "Physics/AirPhysics.h"
#include "Physics/SlopePhysics.h"
#include "Physics/RollingPhysics.h"
#include "Physics/SpindashPhysics.h"
#include "Collision/SensorManager.h"
#include "Collision/CollisionResolver.h"

#include <glm/glm.hpp>
#include <memory>

namespace sclpr::game {

struct MovementInput {
    float moveX{0.0f};
    bool jumpPressed{false};
    bool jumpHeld{false};
    bool agatchPressed{false};
    bool spindashPressed{false};
    bool transformPressed{false};
};

struct MovementState {
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 velocity{0.0f, 0.0f};
    bool grounded{false};
    bool hitCeiling{false};
    bool leftWallColliding{false};
    bool rightWallColliding{false};
    bool inWater{false};
    bool onSlope{false};
    bool isRolling{false};
    bool isSpindashing{false};
    bool isTransformed{false};
    float slopeAngle{0.0f};
    PlayerState playerState{PlayerState::Grounded};
};

class PlayerMovementSystem {
public:
    PlayerMovementSystem();
    ~PlayerMovementSystem() = default;

    void Initialize(float startX, float startY);
    void Update(const MovementInput& input, bool inWater, float dt);
    void Render() const;

    // Getters
    const MovementState& GetMovementState() const noexcept;
    const glm::vec2& GetPosition() const noexcept;
    const glm::vec2& GetVelocity() const noexcept;
    PlayerState GetPlayerState() const noexcept;
    bool IsGrounded() const noexcept;
    bool IsRolling() const noexcept;
    bool IsSpindashing() const noexcept;
    bool IsTransformed() const noexcept;

    // Component access
    sclpr::physics::PhysicsBody& GetPhysicsBody() noexcept;
    sclpr::collision::SensorManager& GetSensorManager() noexcept;
    sclpr::physics::SlopeState& GetSlopeState() noexcept;
    sclpr::physics::RollingState& GetRollingState() noexcept;
    sclpr::physics::SpindashState& GetSpindashState() noexcept;

private:
    void UpdateInput(const MovementInput& input);
    void UpdatePhysics(float dt);
    void UpdateSensors(float dt);
    void ResolveCollisions(float dt);
    void UpdateMovementStates();
    void ApplySlope(float dt);
    void ApplyRolling(float dt);
    void ApplySpindash(float dt);
    void ApplyTransformation(float dt);
    void UpdateStateMachine();

    // Core systems
    std::unique_ptr<sclpr::physics::PhysicsBody> m_physicsBody;
    std::unique_ptr<sclpr::collision::SensorManager> m_sensorManager;
    std::unique_ptr<sclpr::collision::CollisionResolver> m_collisionResolver;
    std::unique_ptr<PlayerStateMachine> m_stateMachine;
    std::unique_ptr<GemstoneSonicController> m_gemstoneController;

    // Physics states
    sclpr::physics::JumpState m_jumpState;
    sclpr::physics::WaterState m_waterState;
    sclpr::physics::SlopeState m_slopeState;
    sclpr::physics::RollingState m_rollingState;
    sclpr::physics::SpindashState m_spindashState;

    // Movement state
    MovementState m_movementState;
    MovementInput m_currentInput;
    bool m_inWater{false};
};

} // namespace sclpr::game
