#pragma once

#include "Game/PlayerStateMachine.h"
#include "Game/GemstoneSonicController.h"
#include "Physics/PhysicsBody.h"
#include "Physics/JumpPhysics.h"
#include "Physics/WaterPhysics.h"
#include "Physics/GroundPhysics.h"
#include "Physics/AirPhysics.h"

#include <glm/glm.hpp>
#include <string>

namespace sclpr::game {

class PlayerController {
public:
    PlayerController();
    ~PlayerController() = default;

    void Initialize(float startX, float startY);
    void Update(float moveInput, bool jumpPressed, bool jumpHeld, bool inWater, float dt);
    void Render() const;

    void SetTransformation(bool enabled);
    void SetGemstoneSonic(bool enabled);

    const glm::vec2& GetPosition() const noexcept;
    const glm::vec2& GetVelocity() const noexcept;
    PlayerState GetState() const noexcept;
    bool IsGrounded() const noexcept;
    bool IsInWater() const noexcept;
    bool IsTransformed() const noexcept;

    sclpr::physics::PhysicsBody& GetPhysicsBody() noexcept;
    const sclpr::physics::PhysicsBody& GetPhysicsBody() const noexcept;

private:
    void UpdateMovement(float moveInput, bool jumpPressed, bool jumpHeld, bool inWater, float dt);
    void ApplyTransformationModifiers(float dt);

    sclpr::physics::PhysicsBody m_physicsBody;
    sclpr::physics::JumpState m_jumpState;
    sclpr::physics::WaterState m_waterState;
    PlayerStateMachine m_stateMachine;
    GemstoneSonicController m_gemstoneController;

    float m_inputX{0.0f};
    float m_baseSpeed{220.0f};
    float m_runSpeed{380.0f};
    float m_jumpStrength{540.0f};
};

} // namespace sclpr::game
