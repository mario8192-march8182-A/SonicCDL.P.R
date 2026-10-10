#include "Game/SceneManager.h"
#include "Game/BootScene.h"
#include "Game/TitleScene.h"

#include <stdexcept>

namespace sclpr::game {

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() = default;

void SceneManager::Initialize(sclpr::renderer::Renderer* renderer) {
    m_renderer = renderer;

    // Create all scenes
    m_scenes[static_cast<int>(sclpr::scene::GameState::BootScreen)] =
        std::make_unique<sclpr::scene::BootScene>();
    m_scenes[static_cast<int>(sclpr::scene::GameState::TitleScreen)] =
        std::make_unique<sclpr::scene::TitleScene>();

    // Initialize boot scene as the first scene
    TransitionToScene(sclpr::scene::GameState::BootScreen);
}

void SceneManager::TransitionToScene(sclpr::scene::GameState state) {
    auto it = m_scenes.find(static_cast<int>(state));
    if (it != m_scenes.end()) {
        m_activeScene = it->second.get();
        m_currentState = state;
        if (m_renderer != nullptr) {
            m_activeScene->Initialize(m_renderer);
        }
    }
}

void SceneManager::Update(float deltaTime) {
    if (m_activeScene != nullptr) {
        m_activeScene->Update(deltaTime);

        // Check if scene wants to transition
        sclpr::scene::GameState nextState = m_activeScene->GetGameState();
        if (nextState != m_currentState) {
            TransitionToScene(nextState);
        }
    }
}

void SceneManager::Render(sclpr::renderer::Renderer* renderer) {
    if (m_activeScene != nullptr) {
        m_activeScene->Render(renderer);
    }
}

void SceneManager::ChangeScene(sclpr::scene::GameState newState) {
    TransitionToScene(newState);
}

sclpr::scene::GameState SceneManager::GetCurrentState() const noexcept {
    return m_currentState;
}

} // namespace sclpr::game
