#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sclpr::renderer {

class Camera2D {
public:
    Camera2D(float width, float height);
    ~Camera2D() = default;

    Camera2D(const Camera2D&) = delete;
    Camera2D& operator=(const Camera2D&) = delete;

    void SetPosition(const glm::vec2& position) noexcept;
    void SetTarget(const glm::vec2& target) noexcept;
    void SetZoom(float zoom) noexcept;
    void SetSize(float width, float height) noexcept;
    void SetBounds(const glm::vec2& min, const glm::vec2& max) noexcept;

    glm::vec2 GetPosition() const noexcept;
    glm::vec2 GetTarget() const noexcept;
    float GetZoom() const noexcept;
    glm::vec2 GetSize() const noexcept;
    glm::vec2 GetMinBounds() const noexcept;
    glm::vec2 GetMaxBounds() const noexcept;

    void Update(float dt, const glm::vec2& playerPosition);

    glm::mat4 GetViewMatrix() const noexcept;
    glm::mat4 GetProjectionMatrix() const noexcept;
    glm::mat4 GetViewProjectionMatrix() const noexcept;

private:
    glm::vec2 m_position{0.0f, 0.0f};
    glm::vec2 m_target{0.0f, 0.0f};
    glm::vec2 m_minBounds{0.0f, 0.0f};
    glm::vec2 m_maxBounds{4096.0f, 2048.0f};
    float m_zoom{1.0f};
    glm::vec2 m_size{1280.0f, 720.0f};
};

} // namespace sclpr::renderer
