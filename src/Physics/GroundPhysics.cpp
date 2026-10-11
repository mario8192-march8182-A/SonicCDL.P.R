#include "Physics/GroundPhysics.h"

namespace sclpr::physics {

void GroundPhysics::Update(PhysicsBody& body, float moveInput, float dt) {
    body.MoveHorizontal(moveInput, dt);
    body.ApplyFriction(dt);

    if (body.IsGrounded()) {
        body.SetVelocity(body.GetVelocity().x, 0.0f);
    }
}

} // namespace sclpr::physics
