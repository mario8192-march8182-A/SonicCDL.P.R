# Sonic CD: Little Planet Revisited

> **Note on Repository Name**: The repository is named "SonicCDL.P.R" but the project is officially called **"Sonic CD: Little Planet Revisited"** (note the double 'L' in "Little"). The repository name was created with a spelling error that couldn't be corrected after creation.

A modern game engine inspired by the Oxygen Engine (Sonic 3 A.I.R.), written from scratch in C++20 using DirectX 12 and SDL3.

## Overview

This project aims to recreate the Sonic CD experience with a custom-built engine that:

- **Does NOT contain or distribute copyrighted SEGA assets**
- Requires users to provide their own Sonic CD ROM/disk image
- Extracts and utilizes resources during runtime
- Provides a modern, modular, and extensible codebase

## Technology Stack

- **Language**: C++20
- **Graphics**: DirectX 12 Ultimate
- **Window/Input**: SDL3
- **Audio**: SDL3_mixer, libogg, libvorbis
- **Compression**: zlib
- **Build System**: CMake
- **IDE**: Visual Studio 2022
- **Platform**: Windows 10/11

## Project Architecture

```
SonicCDL.P.R/
├── src/
│   ├── Core/              # Core engine systems
│   ├── Renderer/          # DirectX 12 rendering
│   ├── Audio/             # Audio management
│   ├── Input/             # Input handling (SDL3)
│   ├── ResourceManager/   # Asset loading and caching
│   ├── Game/              # Game logic
│   ├── Physics/           # Physics simulation
│   ├── Animation/         # Animation system
│   ├── Collision/         # Collision detection
│   ├── Scene/             # Scene management
│   ├── UI/                # UI systems
│   ├── SaveData/          # Save state management
│   ├── ModLoader/         # Mod system
│   └── Tools/             # Development tools
├── include/               # Public headers
├── CMakeLists.txt         # Build configuration
├── CMakePresets.json      # Build presets
└── Mods/                  # Mod directory structure
```

## Current Status

- [x] Project structure and CMake setup
- [x] SDL3 window initialization
- [x] DirectX 12 initialization
- [ ] Sprite Renderer
- [ ] Physics Engine
- [ ] Audio System
- [ ] Collision Detection
- [ ] Resource Manager
- [ ] Level System
- [ ] Mod Loader
- [ ] Development Tools

## Building

### Prerequisites

- Visual Studio 2022 (C++20 support)
- CMake 3.25+
- Git

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/mario8192-march8182-A/SonicCDL.P.R.git
cd SonicCDL.P.R

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022"

# Build
cmake --build . --config Release
```

## Development Philosophy

- **Modular Design**: Each system is independent and loosely coupled
- **Clean Code**: Uses modern C++ practices (smart pointers, RAII, constexpr)
- **No Code Duplication**: DRY principle throughout
- **Clear Documentation**: Every module is well-commented and explained
- **Iterative Development**: Systems are implemented one at a time with thorough explanation

## Running

The engine requires a valid Sonic CD ROM image to be provided by the user at startup. It will extract necessary resources and cache them for runtime use.

## Features (Planned)

### Rendering
- Sprite Renderer
- Camera System
- Texture & Shader Management
- Render Targets
- Sprite Atlases
- Tilemap Rendering
- Parallax Scrolling
- Fullscreen Effects
- Widescreen Support
- Modern Resolution Support

### Physics
- Sonic CD Physics Behavior
- Ground Physics
- Air Physics
- Slope Physics
- Rolling & Spindash
- Jumping
- Springs
- Water Mechanics
- Conveyor Belts
- Moving Platforms

### Collision
- Height Masks
- Angle Maps
- Sensor System
- Floor/Wall/Ceiling Detection

### Audio
- Music Playback
- Sound Effects
- Per-Source Volume Control
- Crossfading
- Looping
- Streaming

### Modern Features
- 60/120/144 FPS / Unlocked FPS
- Save States
- Auto-save
- Debug Menu
- Photo Mode
- Speedrun Timer
- Replay System

### Modding
- Custom Sprites
- Custom Music
- Lua Scripting
- Custom Levels
- Resource Override System

## Code Standards

- Modern C++ (C++20)
- `std::unique_ptr` and `std::shared_ptr` for memory management
- RAII (Resource Acquisition Is Initialization)
- `constexpr` where applicable
- `enum class` for type safety
- Namespaces for organization
- Clear, descriptive comments
- DRY (Don't Repeat Yourself) principle

## Legal Notice

This project is a fan-made engine and does **NOT include any copyrighted SEGA assets**. Users must provide their own Sonic CD ROM/disk image for the engine to function. This engine is for educational and personal use only.

## Contributing

Contributions are welcome! Please ensure:
- Code follows the standards listed above
- Each module remains independent
- New features include documentation
- Commits have clear, descriptive messages

## License

[To be determined - consider MIT, Apache 2.0, or GPL]

## Acknowledgments

- Inspired by the Oxygen Engine (Sonic 3 A.I.R.)
- Built with DirectX 12, SDL3, and modern C++ practices
- Community feedback and contributions
