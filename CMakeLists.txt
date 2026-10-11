cmake_minimum_required(VERSION 3.25)

project(SonicCDLittlePlanetRevisited LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SCLPR_BUILD_TOOLS "Build editor/debug tools" ON)

add_executable(SonicCDLPR
    src/main.cpp
    src/Core/Engine.cpp
    src/Core/Platform/Window.cpp
    src/Core/Timer.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/Camera2D.cpp
    src/Renderer/Texture2D.cpp
    src/Game/Scene.cpp
    src/Game/BootScene.cpp
    src/Game/TitleScene.cpp
    src/Game/SceneManager.cpp
    src/ResourceManager/RomLoader.cpp
    src/ResourceManager/RomExtractor.cpp
    src/ResourceManager/PaletteManager.cpp
    src/ResourceManager/ResourceManager.cpp
    src/Physics/PhysicsBody.cpp
    src/Physics/GroundPhysics.cpp
    src/Physics/AirPhysics.cpp
    src/Physics/PhysicsWorld.cpp
)

target_include_directories(SonicCDLPR
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(SonicCDLPR PRIVATE SDL3::SDL3)

find_package(glm CONFIG REQUIRED)
target_link_libraries(SonicCDLPR PRIVATE glm::glm)

if(WIN32)
    target_link_libraries(SonicCDLPR PRIVATE d3d12 dxgi dxguid)
endif()

if(SCLPR_BUILD_TOOLS)
    message(STATUS "Editor/debug tools enabled.")
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    src/main.cpp
    src/Core/Engine.h
    src/Core/Engine.cpp
    src/Core/Platform/Window.h
    src/Core/Platform/Window.cpp
    src/Core/Timer.h
    src/Core/Timer.cpp
    src/Renderer/Renderer.h
    src/Renderer/Renderer.cpp
    src/Renderer/Camera2D.h
    src/Renderer/Camera2D.cpp
    src/Renderer/Texture2D.h
    src/Renderer/Texture2D.cpp
    src/Renderer/Sprite.h
    src/Game/GameState.h
    src/Game/Scene.h
    src/Game/Scene.cpp
    src/Game/BootScene.h
    src/Game/BootScene.cpp
    src/Game/TitleScene.h
    src/Game/TitleScene.cpp
    src/Game/SceneManager.h
    src/Game/SceneManager.cpp
    src/ResourceManager/RomLoader.h
    src/ResourceManager/RomLoader.cpp
    src/ResourceManager/RomExtractor.h
    src/ResourceManager/RomExtractor.cpp
    src/ResourceManager/PaletteManager.h
    src/ResourceManager/PaletteManager.cpp
    src/ResourceManager/ResourceManager.h
    src/ResourceManager/ResourceManager.cpp
    src/Physics/PhysicsBody.h
    src/Physics/PhysicsBody.cpp
    src/Physics/GroundPhysics.h
    src/Physics/GroundPhysics.cpp
    src/Physics/AirPhysics.h
    src/Physics/AirPhysics.cpp
    src/Physics/PhysicsWorld.h
    src/Physics/PhysicsWorld.cpp
)
