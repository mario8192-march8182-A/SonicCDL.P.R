#include "Core/Engine.h"

#include "Core/Platform/Window.h"
#include "Renderer/Renderer.h"

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
    // The engine is initialized in a staged way:
    // 1. SDL window setup
    // 2. DirectX 12 rendering context
    // 3. game loop start
    // This keeps platform init separate from gameplay logic.
    m_running = true;
}

void Engine::Update() {
    // Placeholder update phase. The gameplay loop will be added here later.
}

void Engine::Render() {
    // Placeholder render phase. Rendering will be handled by the Renderer subsystem.
}

void Engine::Shutdown() {
    m_running = false;
}

} // namespace sclpr::core
