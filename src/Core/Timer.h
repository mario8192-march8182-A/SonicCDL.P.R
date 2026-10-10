#pragma once

#include <cstdint>
#include <chrono>

namespace sclpr::core {

class Timer {
public:
    Timer();

    void Start();
    void Stop();
    void Reset();

    float GetElapsedSeconds() const;
    float GetDeltaTime() const;
    uint64_t GetElapsedMilliseconds() const;

private:
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    float m_deltaTime{0.0f};
    bool m_running{false};
};

} // namespace sclpr::core
