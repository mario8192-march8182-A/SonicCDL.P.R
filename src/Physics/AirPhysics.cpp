#include "Physics/AirPhysics.h"

namespace sclpr::physics {

void AirPhysics::Update(PhysicsBody& body, float dt) {
    body.ApplyGravity(dt);
    body.ClampVelocity();
}

} // namespace sclpr::physics
