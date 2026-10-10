#pragma once

#include "Game/Scene.h"

#include <memory>
#include <vector>

namespace sclpr::scene {

class TitleScene : public Scene {
public:
    explicit TitleScene();
    ~TitleScene() override = default;

    void Initialize(sclpr::renderer::Renderer* renderer) override;
    void Update(float deltaTime) override;
    void Render(sclpr::renderer::Renderer* renderer) override;
    GameState GetGameState() const override;

private:
    enum class TitleState {
        Intro,       // Title intro animation
        Menu,        // Main menu (Press Start)
        LevelSelect, // Transitioning to level select
    };

    TitleState m_state{TitleState::Intro};
    float m_introTimer{0.0f};
    float m_textAlpha{0.0f};
    int m_selectedMenuOption{0};
    bool m_startPressed{false};
};

} // namespace sclpr::scene
