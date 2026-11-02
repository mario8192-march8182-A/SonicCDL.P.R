#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace sclpr::game {

struct HudData {
    int score{0};
    int rings{0};
    int lives{3};
    int time{300};
    bool transformed{false};
    std::string build{"classic"};
};

class HudSystem {
public:
    HudSystem() = default;
    ~HudSystem() = default;

    void Update(const HudData& hudData);
    void Render() const;

    void SetScore(int score);
    void SetRings(int rings);
    void SetLives(int lives);
    void SetTime(int time);
    void SetTransformed(bool transformed);
    void SetBuild(const std::string& build);

    const HudData& GetHudData() const noexcept;

private:
    HudData m_hudData;
};

} // namespace sclpr::game
