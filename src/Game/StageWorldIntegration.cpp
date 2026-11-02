#include "Game/StageWorldIntegration.h"

#include "Game/StageWorld.h"
#include "Game/HudSystem.h"
#include "Game/PlayerController.h"

namespace sclpr::game {

void StageWorldIntegration::BindCameraToPlayer(StageWorld& world, PlayerController& player) {
    // local API hook: world camera is updated by world Update().
    (void)world;
    (void)player;
}

void StageWorldIntegration::UpdateHudFromWorld(StageWorld& world, PlayerController& player, HudSystem& hud) {
    HudData data;
    data.score = 0;
    data.rings = 0;
    data.lives = 3;
    data.time = 300;
    data.transformed = player.IsTransformed();
    data.build = world.GetLevelData().build;
    hud.Update(data);
}

} // namespace sclpr::game
