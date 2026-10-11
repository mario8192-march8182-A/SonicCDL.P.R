#pragma once

#include "Physics/PhysicsBody.h"

namespace sclpr::physics {

class AirPhysics {
public:
    static void Update(PhysicsBody& body, float dt) {
        body.ApplyGravity(dt);
        body.ClampVelocity();
    }
};

} // namespace sclpr::physics
