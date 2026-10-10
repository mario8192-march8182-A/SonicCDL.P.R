#pragma once

#include <string>

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

    bool m_running{false};
    std::string m_title{"Sonic CD: Little Planet Revisited"};
    int m_width{1280};
    int m_height{720};
};

} // namespace sclpr::core
