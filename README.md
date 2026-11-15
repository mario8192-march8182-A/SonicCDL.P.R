# Sonic CD: Little Planet Revisited

A C++20 fan-engine project for experimenting with a 2D Sonic CD-style platform game. This repository contains engine code, not SEGA game data.

> **Status:** prototype/under active development. Some systems have data models and APIs but are not yet connected to a complete playable game. In particular, ROM detection is not asset extraction, and the renderer's sprite/rectangle draw calls are currently placeholders.

## Legal and asset policy

- No copyrighted SEGA ROMs, game assets, videos, music, or extracted data are included.
- Users are responsible for supplying files they are legally entitled to use and for complying with applicable law and platform terms.
- The optional add-on ROM registry only inspects a small cartridge-header region to identify supported titles. It does not extract, copy, or redistribute their contents.
- Add-on ROM registration is not yet a stage importer. Title assets, levels, sprites, and character behavior are not automatically imported from another game's ROM.

## Current implementation snapshot

The repository currently has code for:

- C++20 / CMake project structure, SDL3 window setup, and a Windows Direct3D 12 renderer scaffold.
- Camera and sprite data structures.
- Player movement, physics, collision, stage-data, object, ring/enemy, and mod-related prototypes.
- ROM/build detection and asset/level pipeline prototypes.
- Optional add-on ROM identification for Sonic 2, Sonic 3 & Knuckles, and Sonic 3D Blast.

These are not all production-ready or integrated. In particular:

- `Sprite` is currently a geometry/color descriptor; it has no texture/atlas, animation, or sprite-sheet playback system.
- `Renderer::DrawSprite` and `Renderer::DrawRectangle` are placeholders. A functional sprite renderer still needs GPU resources, shaders/pipelines, texture upload, batching, and draw submission.
- ROM signature scanning is heuristic and is not a verified decoder for Sonic CD or other games' asset formats.
- Add-on recognition can use ROM header text and/or a filename hint; verify the detected title yourself. It does not prove a specific regional/revision dump.
- Tails/Knuckles playable-character support and Sonic 2 spin-dash behavior are not implemented by registering an add-on ROM alone.
- Sonic 3D Blast stages would require a separately designed 2D conversion/import pipeline; this project does not convert them yet.

## Optional add-on ROM registry

The `AddOnRomManager` accepts a user-selected cartridge image and can report a recognized title and intended reference capabilities:

| Add-on source | Intended reference use | Current status |
| --- | --- | --- |
| Sonic 3 & Knuckles | Knuckles and stage-design reference | Header/title recognition only; no character or stage import |
| Sonic 2 | Tails and spin-dash reference | Header/title recognition only; no character or move import |
| Sonic 3D Blast | Stage-design reference for a 2D reinterpretation | Header/title recognition only; no stage conversion |

Use the API from `ResourceManager/AddOnRomManager.h`. Registration stores the selected file path and metadata, not a copy of the ROM. The supported image header check expects a readable Mega Drive/Genesis-style header; interleaved SMD images are not decoded.

## Build

### Requirements

- CMake 3.25 or newer
- C++20 compiler
- SDL3 CMake package
- GLM CMake package
- nlohmann_json CMake package
- Windows for the current Direct3D 12 renderer scaffold

```sh
git clone https://github.com/mario8192-march8182-A/SonicCDL.P.R.git
cd SonicCDL.P.R
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

## Project layout

```text
src/
├── Core/              # Engine and platform setup
├── Renderer/          # Renderer, camera, textures, sprite descriptor
├── Game/              # Player, scenes, stage world, gameplay prototypes
├── ResourceManager/   # ROM/asset pipeline and add-on ROM registry
├── Physics/           # Movement physics
├── Collision/         # Collision sensors and resolution
└── Mod/               # Mod metadata/loading prototype
```

## Planned work

- Implement a real textured sprite renderer, sprite sheets, and animation playback.
- Replace heuristic ROM scanning with documented, format-specific parsers and validation.
- Add stage/object import formats that use user-provided, legally obtained data.
- Connect the stage world, player interactions, camera, HUD, and scene flow into a playable loop.
- Implement opt-in, independently authored playable-character and 2D stage-conversion systems.

## Contributing

Keep engine code separate from copyrighted game data. Clearly label prototypes and avoid claiming that a format is supported until it has been tested against known-good, legally obtained samples.
