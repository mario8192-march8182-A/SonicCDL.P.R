#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace sclpr::game {

class StageWorld;
class PlayerController;

class StageWorldIntegration {
public:
    static void BindCameraToPlayer(StageWorld& world, PlayerController& player);
    static void UpdateHudFromWorld(StageWorld& world, PlayerController& player, class HudSystem& hud);
};

} // namespace sclpr::game
