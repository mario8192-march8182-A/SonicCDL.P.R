#include "Game/HudSystem.h"

namespace sclpr::game {

void HudSystem::Update(const HudData& hudData) {
    m_hudData = hudData;
}

void HudSystem::Render() const {
    // HUD rendering is intentionally deferred to the renderer/UI layer.
}

void HudSystem::SetScore(int score) {
    m_hudData.score = score;
}

void HudSystem::SetRings(int rings) {
    m_hudData.rings = rings;
}

void HudSystem::SetLives(int lives) {
    m_hudData.lives = lives;
}

void HudSystem::SetTime(int time) {
    m_hudData.time = time;
}

void HudSystem::SetTransformed(bool transformed) {
    m_hudData.transformed = transformed;
}

void HudSystem::SetBuild(const std::string& build) {
    m_hudData.build = build;
}

const HudData& HudSystem::GetHudData() const noexcept {
    return m_hudData;
}

} // namespace sclpr::game
