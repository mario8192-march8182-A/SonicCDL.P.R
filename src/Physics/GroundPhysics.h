#pragma once

#include "Physics/PhysicsBody.h"

namespace sclpr::physics {

class GroundPhysics {
public:
    static void Update(PhysicsBody& body, float moveInput, float dt) {
        body.MoveHorizontal(moveInput, dt);
        body.ApplyFriction(dt);

        if (body.IsGrounded()) {
            body.SetVelocity(body.GetVelocity().x, 0.0f);
        }
    }
};

class AirPhysics {
public:
    static void Update(PhysicsBody& body, float dt) {
        body.ApplyGravity(dt);
        body.ClampVelocity();
    }
};

} // namespace sclpr::physics
