#pragma once

#include <glm/glm.hpp>

namespace sclpr::physics {

class PhysicsBody {
public:
    PhysicsBody();
    ~PhysicsBody() = default;

    PhysicsBody(const PhysicsBody&) = delete;
    PhysicsBody& operator=(const PhysicsBody&) = delete;

    void SetPosition(float x, float y) noexcept;
    void SetPosition(const glm::vec2& position) noexcept;
    void SetVelocity(float x, float y) noexcept;
    void SetVelocity(const glm::vec2& velocity) noexcept;
    void SetGrounded(bool grounded) noexcept;
    void SetGravity(float gravity) noexcept;
    void SetGroundAcceleration(float acceleration) noexcept;
    void SetAirAcceleration(float acceleration) noexcept;
    void SetJumpStrength(float jumpStrength) noexcept;
    void SetMaxGroundSpeed(float maxSpeed) noexcept;
    void SetMaxAirSpeed(float maxSpeed) noexcept;

    const glm::vec2& GetPosition() const noexcept;
    const glm::vec2& GetVelocity() const noexcept;
    bool IsGrounded() const noexcept;
    float GetGravity() const noexcept;
    float GetGroundAcceleration() const noexcept;
    float GetAirAcceleration() const noexcept;
    float GetJumpStrength() const noexcept;
    float GetMaxGroundSpeed() const noexcept;
    float GetMaxAirSpeed() const noexcept;

    void MoveHorizontal(float input, float dt);
    void ApplyGravity(float dt);
    void Jump();
    void ApplyFriction(float dt);
    void ClampVelocity();

private:
    glm::vec2 m_position{0.0f, 0.0f};
    glm::vec2 m_velocity{0.0f, 0.0f};

    bool m_grounded{false};
    float m_gravity{1800.0f};
    float m_groundAcceleration{1800.0f};
    float m_airAcceleration{900.0f};
    float m_jumpStrength{620.0f};
    float m_maxGroundSpeed{440.0f};
    float m_maxAirSpeed{520.0f};
    float m_groundFriction{1400.0f};
};

} // namespace sclpr::physics
