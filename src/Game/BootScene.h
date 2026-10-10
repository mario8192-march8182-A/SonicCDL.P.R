#pragma once

#include "Game/Scene.h"
#include <memory>

namespace sclpr::scene {

class BootScene : public Scene {
public:
    explicit BootScene();
    ~BootScene() override = default;

    void Initialize(sclpr::renderer::Renderer* renderer) override;
    void Update(float deltaTime) override;
    void Render(sclpr::renderer::Renderer* renderer) override;
    GameState GetGameState() const override;

private:
    enum class BootPhase {
        SEGA,           // "Original Game By SEGA"
        Remaster,       // "Unofficial Remaster By ALTGR109!!"
        ROMLoading,     // ROM/Disk image loading screen
        Complete,       // Boot complete, transition to title
    };

    BootPhase m_phase{BootPhase::SEGA};
    float m_phaseTimer{0.0f};
    float m_textAlpha{0.0f};
    bool m_transitioningOut{false};
};

} // namespace sclpr::scene
