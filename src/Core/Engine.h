#pragma once

#include <memory>
#include <string>

namespace sclpr::core {

class Window;

} // namespace sclpr::core

namespace sclpr::renderer {

class Renderer;

} // namespace sclpr::renderer

namespace sclpr::game {

class SceneManager;

} // namespace sclpr::game

namespace sclpr::core {

class Engine {
public:
    explicit Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    int Run();

private:
    void Initialize();
    void Update();
    void Render();
    void Shutdown();

    std::unique_ptr<Window> m_window;
    std::unique_ptr<sclpr::renderer::Renderer> m_renderer;
    std::unique_ptr<sclpr::game::SceneManager> m_sceneManager;

    bool m_running{false};
    std::string m_title{"Sonic CD: Little Planet Revisited"};
    int m_width{1280};
    int m_height{720};
};

} // namespace sclpr::core
