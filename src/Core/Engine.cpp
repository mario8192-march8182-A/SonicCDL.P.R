#include "Core/Engine.h"

#include <memory>

namespace sclpr::core {

Engine::Engine() = default;

Engine::~Engine() {
    Shutdown();
}

int Engine::Run() {
    Initialize();

    while (m_running) {
        Update();
        Render();
    }

    Shutdown();
    return 0;
}

void Engine::Initialize() {
    m_window = std::make_unique<Window>(m_title, m_width, m_height);
    m_window->Show();

    m_renderer = std::make_unique<sclpr::renderer::Renderer>(m_window->GetNativeHandle());
    m_renderer->Initialize();

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
        m_renderer->EndFrame();
    }
}

void Engine::Shutdown() {
    m_renderer.reset();
    m_window.reset();
    m_running = false;
}

} // namespace sclpr::core
