#pragma once

#include <cstdint>
#include <string>

namespace sclpr::game {

enum class PlayerState : std::uint8_t {
    Grounded,
    Air,
    InWater,
    Transformed,
    Hurt,
    Dead
};

struct PlayerStateInfo {
    PlayerState state{PlayerState::Grounded};
    bool grounded{false};
    bool inWater{false};
    bool transformed{false};
    bool jumpQueued{false};
    float transformationTimer{0.0f};
    std::string transformationName;
};

class PlayerStateMachine {
public:
    PlayerStateMachine() = default;
    ~PlayerStateMachine() = default;

    void Update(bool grounded, bool inWater, bool transformRequested, float dt);
    PlayerState GetState() const noexcept;
    const PlayerStateInfo& GetInfo() const noexcept;

    void RequestTransformation(const std::string& transformationName);
    void ClearTransformation();

private:
    PlayerStateInfo m_info;
};

} // namespace sclpr::game
