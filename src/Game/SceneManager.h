#pragma once

#include "Game/GameState.h"
#include "Game/Scene.h"
#include "Renderer/Renderer.h"
#include "Core/Timer.h"

#include <memory>
#include <unordered_map>

namespace sclpr::game {

class SceneManager {
public:
    explicit SceneManager();
    ~SceneManager();

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    void Initialize(sclpr::renderer::Renderer* renderer);
    void Update(float deltaTime);
    void Render(sclpr::renderer::Renderer* renderer);

    void ChangeScene(sclpr::scene::GameState newState);
    sclpr::scene::GameState GetCurrentState() const noexcept;

private:
    void TransitionToScene(sclpr::scene::GameState state);

    std::unordered_map<int, std::unique_ptr<sclpr::scene::Scene>> m_scenes;
    sclpr::scene::GameState m_currentState{sclpr::scene::GameState::BootScreen};
    sclpr::scene::Scene* m_activeScene{nullptr};
    sclpr::renderer::Renderer* m_renderer{nullptr};
};

} // namespace sclpr::game
