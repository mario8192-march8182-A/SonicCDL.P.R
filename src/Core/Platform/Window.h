#pragma once

#include <SDL3/SDL.h>

#include <string>

namespace sclpr::core {

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool IsOpen() const noexcept;
    void PollEvents();
    void Show();
    int GetWidth() const noexcept;
    int GetHeight() const noexcept;
    SDL_Window* GetNativeHandle() const noexcept;

private:
    SDL_Window* m_window{nullptr};
    std::string m_title;
    int m_width{0};
    int m_height{0};
    bool m_open{false};
};

} // namespace sclpr::core
