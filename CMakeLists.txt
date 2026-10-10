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
    src/Renderer/Renderer.cpp
)

target_include_directories(SonicCDLPR
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(SonicCDLPR PRIVATE SDL3::SDL3)

if(WIN32)
    target_link_libraries(SonicCDLPR PRIVATE d3d12 dxgi dxguid)
endif()

# Future module structure hook points.
# The project is intentionally organized in a way that each subsystem can grow independently.
if(SCLPR_BUILD_TOOLS)
    message(STATUS "Editor/debug tools enabled.")
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    src/main.cpp
    src/Core/Engine.h
    src/Core/Engine.cpp
    src/Core/Platform/Window.h
    src/Core/Platform/Window.cpp
    src/Renderer/Renderer.h
    src/Renderer/Renderer.cpp
)
