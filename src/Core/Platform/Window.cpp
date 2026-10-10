#include "Core/Platform/Window.h"

#include <SDL3/SDL.h>

#include <stdexcept>

namespace sclpr::core {

Window::Window(const std::string& title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD) != 0) {
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());
    }

    m_window = SDL_CreateWindow(
        m_title.c_str(),
        m_width,
        m_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
    );

    if (m_window == nullptr) {
        SDL_Quit();
        throw std::runtime_error(std::string("Failed to create SDL window: ") + SDL_GetError());
    }

    m_open = true;
}

Window::~Window() {
    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
}

bool Window::IsOpen() const noexcept {
    return m_open;
}

void Window::PollEvents() {
    SDL_Event event{};
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                m_open = false;
                break;
            default:
                break;
        }
    }
}

void Window::Show() {
    if (m_window != nullptr) {
        SDL_ShowWindow(m_window);
    }
}

int Window::GetWidth() const noexcept {
    return m_width;
}

int Window::GetHeight() const noexcept {
    return m_height;
}

SDL_Window* Window::GetNativeHandle() const noexcept {
    return m_window;
}

} // namespace sclpr::core
