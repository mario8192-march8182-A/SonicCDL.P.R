#include "Game/BootScene.h"
#include "ResourceManager/ResourceManager.h"

#include <glm/glm.hpp>
#include <memory>

namespace sclpr::scene {

BootScene::BootScene() : Scene(), m_romLoader(nullptr) {}

void BootScene::Initialize(sclpr::renderer::Renderer* renderer) {
    // Initialize boot screen resources
    m_phase = BootPhase::SEGA;
    m_phaseTimer = 0.0f;
    m_textAlpha = 0.0f;
    m_romLoadingProgress = 0.0f;
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
                StartRomLoading();
            }
            break;

        case BootPhase::ROMLoading:
            // ROM loading screen with progress indicator
            m_textAlpha = 1.0f;
            UpdateRomLoading(deltaTime);
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

void BootScene::StartRomLoading() {
    if (m_romLoader == nullptr) {
        // Create resource manager and rom loader
        m_resourceManager = std::make_unique<sclpr::resource::ResourceManager>();
        m_resourceManager->Initialize();
        m_romLoader = m_resourceManager->GetRomLoader();
    }

    // Detect available ROMs
    auto detectedRoms = m_romLoader->DetectRoms();

    if (!detectedRoms.empty()) {
        // Load the first valid ROM found
        // In a real implementation, this would show a ROM selection menu
        m_selectedRomPath = detectedRoms[0].filename;
        m_romLoadingActive = true;
    } else {
        // No ROM found - show error and proceed anyway (for testing)
        m_romLoadingActive = false;
        m_phase = BootPhase::Complete;
    }
}

void BootScene::UpdateRomLoading(float deltaTime) {
    if (!m_romLoadingActive) {
        m_phase = BootPhase::Complete;
        return;
    }

    // Simulate ROM loading progress over 2 seconds
    float loadDuration = 2.0f;
    m_romLoadingProgress = std::min(1.0f, m_phaseTimer / loadDuration);

    // Attempt to load ROM when progress reaches 100%
    if (m_romLoadingProgress >= 1.0f) {
        if (m_romLoader->LoadRom(m_selectedRomPath)) {
            // ROM loaded successfully
            m_romLoadedSuccessfully = true;
        }
        m_romLoadingActive = false;
        m_phaseTimer = 0.0f;
        m_phase = BootPhase::Complete;
    }
}

void BootScene::Render(sclpr::renderer::Renderer* renderer) {
    // Clear the screen to black
    renderer->Clear(0.0f, 0.0f, 0.0f, 1.0f);

    switch (m_phase) {
        case BootPhase::SEGA:
            // Render "Original Game By SEGA" text
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
            // Loading bar (progress)
            renderer->DrawRectangle(640.0f - 250.0f, 360.0f + 50.0f, 500.0f * m_romLoadingProgress, 20.0f,
                                   glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            // Loading bar background
            renderer->DrawRectangle(640.0f - 250.0f, 360.0f + 50.0f, 500.0f, 20.0f,
                                   glm::vec4(0.3f, 0.3f, 0.3f, 0.5f));
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
