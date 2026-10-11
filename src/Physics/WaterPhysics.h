#pragma once

#include <algorithm>

#include "Physics/PhysicsBody.h"

namespace sclpr::physics {

struct WaterState {
    bool inWater{false};
    float currentDepth{0.0f};
    float swimForce{140.0f};
    float buoyancy{920.0f};
    float drag{0.82f};
    float maxSwimSpeed{220.0f};
};

class WaterPhysics {
public:
    static void Update(PhysicsBody& body, WaterState& state, float moveInput, float dt);
    static void Reset(WaterState& state);
};

} // namespace sclpr::physics
