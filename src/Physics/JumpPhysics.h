#pragma once

#include <algorithm>
#include <cmath>

#include "Physics/PhysicsBody.h"

namespace sclpr::physics {

struct JumpState {
    bool jumpQueued{false};
    bool jumpHeld{false};
    bool jumpWasHeld{false};
    float coyoteTimer{0.0f};
    float jumpBufferTimer{0.0f};
    float jumpHoldTime{0.0f};
    float jumpCutGravity{0.0f};
};

class JumpPhysics {
public:
    static constexpr float kDefaultCoyoteTime = 0.10f;
    static constexpr float kDefaultJumpBuffer = 0.12f;
    static constexpr float kJumpCutMultiplier = 0.55f;

    static void Update(PhysicsBody& body, JumpState& state, bool jumpPressed, bool jumpHeld, float dt);
    static void Reset(JumpState& state);
};

} // namespace sclpr::physics
