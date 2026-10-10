#include "Game/TitleScene.h"

#include <glm/glm.hpp>

namespace sclpr::scene {

TitleScene::TitleScene() : Scene() {}

void TitleScene::Initialize(sclpr::renderer::Renderer* renderer) {
    // Initialize title screen resources
    m_state = TitleState::Intro;
    m_introTimer = 0.0f;
    m_textAlpha = 0.0f;
}

void TitleScene::Update(float deltaTime) {
    m_introTimer += deltaTime;

    switch (m_state) {
        case TitleState::Intro:
            // Fade in the title
            if (m_introTimer < 1.0f) {
                m_textAlpha = m_introTimer;  // Fade in over 1 second
            } else {
                m_textAlpha = 1.0f;
                m_state = TitleState::Menu;
                m_introTimer = 0.0f;
            }
            break;

        case TitleState::Menu:
            // Menu is active, waiting for input
            // Input will be handled by the engine's input system
            m_textAlpha = 1.0f;
            break;

        case TitleState::LevelSelect:
            // Transition to level select
            if (m_introTimer < 0.5f) {
                m_textAlpha = 1.0f - (m_introTimer / 0.5f);
            } else {
                m_startPressed = true;
            }
            break;
    }
}

void TitleScene::Render(sclpr::renderer::Renderer* renderer) {
    // Clear the screen
    renderer->Clear(0.1f, 0.1f, 0.1f, 1.0f);

    // Render title background (placeholder)
    renderer->DrawRectangle(0.0f, 0.0f, 1280.0f, 720.0f,
                           glm::vec4(0.05f, 0.05f, 0.15f, 1.0f));

    // Render title text
    renderer->DrawRectangle(640.0f - 300.0f, 150.0f, 600.0f, 150.0f,
                           glm::vec4(1.0f, 0.84f, 0.0f, m_textAlpha));

    // Render "Press Start" text
    if (m_state == TitleState::Menu) {
        float blinkAlpha = 0.5f + 0.5f * glm::sin(m_introTimer * 3.14159f * 2.0f);
        renderer->DrawRectangle(640.0f - 150.0f, 400.0f, 300.0f, 50.0f,
                               glm::vec4(1.0f, 1.0f, 1.0f, blinkAlpha));
    }

    // Render menu options (placeholder)
    if (m_state == TitleState::Menu) {
        renderer->DrawRectangle(640.0f - 100.0f, 500.0f, 200.0f, 40.0f,
                               glm::vec4(0.0f, 1.0f, 0.0f, m_textAlpha));
        renderer->DrawRectangle(640.0f - 100.0f, 550.0f, 200.0f, 40.0f,
                               glm::vec4(0.5f, 0.5f, 0.5f, m_textAlpha));
    }
}

GameState TitleScene::GetGameState() const {
    return m_startPressed ? GameState::LevelSelect : GameState::TitleScreen;
}

} // namespace sclpr::scene
