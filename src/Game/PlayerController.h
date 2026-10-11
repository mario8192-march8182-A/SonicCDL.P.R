#pragma once

#include "Physics/PhysicsBody.h"
#include "Physics/JumpPhysics.h"
#include "Physics/WaterPhysics.h"
#include "Physics/GroundPhysics.h"
#include "Physics/AirPhysics.h"

#include <glm/glm.hpp>
#include <cstdint>

namespace sclpr::game {

enum class PlayerState : uint8_t {
    Idle,
    Walking,
    Running,
    Jumping,
    Falling,
    InWater,
    Rolling,
    Spindashing,
    Transformed,
    Hurt,
    Dead
};

class PlayerController {
public:
    explicit PlayerController();
    ~PlayerController() = default;

    PlayerController(const PlayerController&) = delete;
    PlayerController& operator=(const PlayerController&) = delete;

    void Initialize(float startX, float startY);
    void Update(float moveInput, bool jumpPressed, bool jumpHeld, bool inWater, float dt);
    void Render() const;

    const glm::vec2& GetPosition() const noexcept;
    const glm::vec2& GetVelocity() const noexcept;
    PlayerState GetState() const noexcept;
    bool IsGrounded() const noexcept;
    bool IsInWater() const noexcept;

    sclpr::physics::PhysicsBody& GetPhysicsBody() noexcept;
    const sclpr::physics::PhysicsBody& GetPhysicsBody() const noexcept;

private:
    void UpdateState(float moveInput, bool jumpPressed, float dt);
    void ApplyMovement(float dt);

    sclpr::physics::PhysicsBody m_physicsBody;
    sclpr::physics::JumpState m_jumpState;
    sclpr::physics::WaterState m_waterState;
    PlayerState m_state{PlayerState::Idle};
    float m_inputX{0.0f};
};

} // namespace sclpr::game
