#include "Physics/WaterPhysics.h"

namespace sclpr::physics {

void WaterPhysics::Update(PhysicsBody& body, WaterState& state, float moveInput, float dt) {
    if (!state.inWater) {
        return;
    }

    // Water reduces gravity and adds buoyancy.
    const float gravity = body.GetGravity() * 0.32f;
    body.SetGravity(gravity);

    // Horizontal controls are damped in water.
    const float swimAcceleration = state.swimForce * dt;
    body.SetVelocity(body.GetVelocity().x + moveInput * swimAcceleration, body.GetVelocity().y);

    // Vertical motion is biased upward by buoyancy.
    if (body.GetVelocity().y > 0.0f) {
        body.SetVelocity(body.GetVelocity().x, body.GetVelocity().y * state.drag);
    }
    body.SetVelocity(body.GetVelocity().x, body.GetVelocity().y - state.buoyancy * dt);

    // Keep swim speed in check.
    if (std::abs(body.GetVelocity().x) > state.maxSwimSpeed) {
        body.SetVelocity(std::copysign(state.maxSwimSpeed, body.GetVelocity().x), body.GetVelocity().y);
    }
}

void WaterPhysics::Reset(WaterState& state) {
    state.inWater = false;
    state.currentDepth = 0.0f;
    state.swimForce = 140.0f;
    state.buoyancy = 920.0f;
    state.drag = 0.82f;
    state.maxSwimSpeed = 220.0f;
}

} // namespace sclpr::physics
