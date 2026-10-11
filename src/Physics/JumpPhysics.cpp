#include "Physics/JumpPhysics.h"

namespace sclpr::physics {

void JumpPhysics::Update(PhysicsBody& body, JumpState& state, bool jumpPressed, bool jumpHeld, float dt) {
    // Sonic-like coyote time: allows a jump slightly after leaving the floor.
    if (body.IsGrounded()) {
        state.coyoteTimer = kDefaultCoyoteTime;
    } else {
        state.coyoteTimer = std::max(0.0f, state.coyoteTimer - dt);
    }

    if (jumpPressed) {
        state.jumpBufferTimer = kDefaultJumpBuffer;
    } else {
        state.jumpBufferTimer = std::max(0.0f, state.jumpBufferTimer - dt);
    }

    // If the jump buffer and coyote timer overlap, trigger the jump.
    const bool canJump = state.jumpBufferTimer > 0.0f && state.coyoteTimer > 0.0f;
    if (canJump && !state.jumpWasHeld) {
        body.SetVelocity(body.GetVelocity().x, -body.GetJumpStrength());
        body.SetGrounded(false);
        state.jumpHeld = true;
        state.jumpHoldTime = 0.0f;
        state.jumpBufferTimer = 0.0f;
        state.coyoteTimer = 0.0f;
    }

    state.jumpWasHeld = jumpHeld;
    state.jumpHeld = jumpHeld;

    // Shorter jump when the button is released early.
    if (!jumpHeld && body.GetVelocity().y < 0.0f) {
        const float cutVelocity = body.GetGravity() * kJumpCutMultiplier * dt;
        body.SetVelocity(body.GetVelocity().x, body.GetVelocity().y + cutVelocity);
    }

    // Jump hold: slightly increases hang time for a more responsive feel.
    if (jumpHeld && body.GetVelocity().y < 0.0f) {
        state.jumpHoldTime += dt;
        if (state.jumpHoldTime < 0.12f) {
            body.SetVelocity(body.GetVelocity().x, body.GetVelocity().y + (body.GetGravity() * 0.25f * dt));
        }
    }
}

void JumpPhysics::Reset(JumpState& state) {
    state.jumpQueued = false;
    state.jumpHeld = false;
    state.jumpWasHeld = false;
    state.coyoteTimer = 0.0f;
    state.jumpBufferTimer = 0.0f;
    state.jumpHoldTime = 0.0f;
}

} // namespace sclpr::physics
