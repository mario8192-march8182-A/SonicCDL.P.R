#include "Core/Engine.h"

#include "Core/Platform/Window.h"
#include "Renderer/Renderer.h"
#include "Game/SceneManager.h"
#include "Core/Timer.h"

#include <memory>

namespace sclpr::core {

Engine::Engine() = default;

Engine::~Engine() {
    Shutdown();
}

int Engine::Run() {
    Initialize();

    Timer gameTimer;
    gameTimer.Start();
    float lastTime = gameTimer.GetElapsedSeconds();

    while (m_running) {
        float currentTime = gameTimer.GetElapsedSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        Update();
        m_sceneManager->Update(deltaTime);
        Render();
    }

    Shutdown();
    return 0;
}

void Engine::Initialize() {
    // Initialize platform layer
    m_window = std::make_unique<Window>(m_title, m_width, m_height);
    m_window->Show();

    // Initialize renderer
    m_renderer = std::make_unique<sclpr::renderer::Renderer>(m_window->GetNativeHandle());
    m_renderer->Initialize();

    // Initialize scene manager
    m_sceneManager = std::make_unique<sclpr::game::SceneManager>();
    m_sceneManager->Initialize(m_renderer.get());

    m_running = m_window->IsOpen();
}

void Engine::Update() {
    if (m_window != nullptr) {
        m_window->PollEvents();
        m_running = m_window->IsOpen();
    }
}

void Engine::Render() {
    if (m_renderer != nullptr) {
        m_renderer->BeginFrame();
        m_sceneManager->Render(m_renderer.get());
        m_renderer->EndFrame();
    }
}

void Engine::Shutdown() {
    m_sceneManager.reset();
    m_renderer.reset();
    m_window.reset();
    m_running = false;
}

} // namespace sclpr::core
