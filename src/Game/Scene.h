#pragma once

#include "Game/GameState.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera2D.h"
#include "Core/Timer.h"

#include <memory>

namespace sclpr::scene {

class Scene {
public:
    explicit Scene();
    virtual ~Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    virtual void Initialize(sclpr::renderer::Renderer* renderer) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(sclpr::renderer::Renderer* renderer) = 0;
    virtual GameState GetGameState() const = 0;
};

} // namespace sclpr::scene
