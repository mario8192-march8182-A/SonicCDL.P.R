#include "Game/PlayerStateMachine.h"

namespace sclpr::game {

void PlayerStateMachine::Update(bool grounded, bool inWater, bool transformRequested, float dt) {
    m_info.grounded = grounded;
    m_info.inWater = inWater;

    if (m_info.transformed && m_info.transformationTimer > 0.0f) {
        m_info.transformationTimer -= dt;
    } else if (m_info.transformed) {
        m_info.transformed = false;
        m_info.transformationName.clear();
    }

    if (transformRequested && !m_info.transformed) {
        m_info.transformed = true;
        m_info.transformationName = "Gemstone Sonic";
        m_info.transformationTimer = 30.0f;
    }

    if (m_info.inWater) {
        m_info.state = PlayerState::InWater;
    } else if (!m_info.grounded) {
        m_info.state = PlayerState::Air;
    } else if (m_info.transformed) {
        m_info.state = PlayerState::Transformed;
    } else {
        m_info.state = PlayerState::Grounded;
    }
}

PlayerState PlayerStateMachine::GetState() const noexcept {
    return m_info.state;
}

const PlayerStateInfo& PlayerStateMachine::GetInfo() const noexcept {
    return m_info;
}

void PlayerStateMachine::RequestTransformation(const std::string& transformationName) {
    m_info.transformed = true;
    m_info.transformationName = transformationName;
    m_info.transformationTimer = 30.0f;
}

void PlayerStateMachine::ClearTransformation() {
    m_info.transformed = false;
    m_info.transformationName.clear();
    m_info.transformationTimer = 0.0f;
}

} // namespace sclpr::game
