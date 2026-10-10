#pragma once

#include <cstdint>

namespace sclpr::scene {

enum class GameState : uint8_t {
    BootScreen,    // SEGA and Remaster splash screens
    TitleScreen,   // Main title screen
    LevelSelect,   // Level/time zone selection
    Playing,       // Gameplay
    Paused,        // Game paused
    GameOver,      // Game over screen
    StaffRoll,     // Credits
};

} // namespace sclpr::scene
