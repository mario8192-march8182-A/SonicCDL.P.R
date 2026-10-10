#include "Game/BootScene.h"

#include <glm/glm.hpp>

namespace sclpr::scene {

BootScene::BootScene() : Scene() {}

void BootScene::Initialize(sclpr::renderer::Renderer* renderer) {
    // Initialize boot screen resources
    m_phase = BootPhase::SEGA;
    m_phaseTimer = 0.0f;
    m_textAlpha = 0.0f;
}

void BootScene::Update(float deltaTime) {
    m_phaseTimer += deltaTime;

    switch (m_phase) {
        case BootPhase::SEGA:
            // Fade in: 0.5s, display: 2.0s, fade out: 0.5s
            if (m_phaseTimer < 0.5f) {
                m_textAlpha = m_phaseTimer / 0.5f;  // Fade in
            } else if (m_phaseTimer < 2.5f) {
                m_textAlpha = 1.0f;  // Full display
            } else if (m_phaseTimer < 3.0f) {
                m_textAlpha = 1.0f - ((m_phaseTimer - 2.5f) / 0.5f);  // Fade out
            } else {
                m_phase = BootPhase::Remaster;
                m_phaseTimer = 0.0f;
                m_textAlpha = 0.0f;
            }
            break;

        case BootPhase::Remaster:
            // Fade in: 0.5s, display: 2.0s, fade out: 0.5s
            if (m_phaseTimer < 0.5f) {
                m_textAlpha = m_phaseTimer / 0.5f;
            } else if (m_phaseTimer < 2.5f) {
                m_textAlpha = 1.0f;
            } else if (m_phaseTimer < 3.0f) {
                m_textAlpha = 1.0f - ((m_phaseTimer - 2.5f) / 0.5f);
            } else {
                m_phase = BootPhase::ROMLoading;
                m_phaseTimer = 0.0f;
                m_textAlpha = 0.0f;
            }
            break;

        case BootPhase::ROMLoading:
            // ROM loading screen with progress indicator
            m_textAlpha = 1.0f;
            if (m_phaseTimer > 2.0f) {  // Simulate ROM loading
                m_phase = BootPhase::Complete;
                m_phaseTimer = 0.0f;
            }
            break;

        case BootPhase::Complete:
            // Brief fade out before transition to title screen
            if (m_phaseTimer < 0.5f) {
                m_textAlpha = 1.0f - (m_phaseTimer / 0.5f);
            } else {
                m_transitioningOut = true;
            }
            break;
    }
}

void BootScene::Render(sclpr::renderer::Renderer* renderer) {
    // Clear the screen to black
    renderer->Clear(0.0f, 0.0f, 0.0f, 1.0f);

    switch (m_phase) {
        case BootPhase::SEGA:
            // Render "Original Game By SEGA" text
            // This is a placeholder. Actual text rendering will use SDL_ttf or a font texture.
            renderer->DrawRectangle(640.0f - 200.0f, 360.0f - 50.0f, 400.0f, 100.0f,
                                   glm::vec4(1.0f, 1.0f, 1.0f, m_textAlpha));
            break;

        case BootPhase::Remaster:
            // Render "Unofficial Remaster By ALTGR109!!" text
            renderer->DrawRectangle(640.0f - 250.0f, 360.0f - 50.0f, 500.0f, 100.0f,
                                   glm::vec4(1.0f, 1.0f, 0.0f, m_textAlpha));
            break;

        case BootPhase::ROMLoading:
            // Render ROM loading screen
            renderer->DrawRectangle(640.0f - 300.0f, 360.0f - 100.0f, 600.0f, 200.0f,
                                   glm::vec4(0.2f, 0.2f, 0.3f, 1.0f));
            // Loading bar (simulated)
            float loadProgress = m_phaseTimer / 2.0f;
            renderer->DrawRectangle(640.0f - 250.0f, 360.0f + 50.0f, 500.0f * loadProgress, 20.0f,
                                   glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            break;

        case BootPhase::Complete:
            // Fade to black
            renderer->DrawRectangle(0.0f, 0.0f, 1280.0f, 720.0f,
                                   glm::vec4(0.0f, 0.0f, 0.0f, 1.0f - m_textAlpha));
            break;
    }
}

GameState BootScene::GetGameState() const {
    return m_transitioningOut ? GameState::TitleScreen : GameState::BootScreen;
}

} // namespace sclpr::scene
