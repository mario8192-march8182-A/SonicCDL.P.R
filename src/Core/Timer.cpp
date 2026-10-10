#include "Core/Timer.h"

namespace sclpr::core {

Timer::Timer() {
    Reset();
}

void Timer::Start() {
    m_running = true;
    m_startTime = std::chrono::high_resolution_clock::now();
}

void Timer::Stop() {
    m_running = false;
}

void Timer::Reset() {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_lastFrameTime = m_startTime;
    m_deltaTime = 0.0f;
}

float Timer::GetElapsedSeconds() const {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - m_startTime;
    return elapsed.count();
}

float Timer::GetDeltaTime() const {
    return m_deltaTime;
}

uint64_t Timer::GetElapsedMilliseconds() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTime);
    return static_cast<uint64_t>(elapsed.count());
}

} // namespace sclpr::core
