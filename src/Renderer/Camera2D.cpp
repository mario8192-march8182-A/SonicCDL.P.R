#include "Renderer/Camera2D.h"

namespace sclpr::renderer {

Camera2D::Camera2D(float width, float height)
    : m_size(width, height) {}

void Camera2D::SetPosition(const glm::vec2& position) noexcept {
    m_position = position;
}

void Camera2D::SetTarget(const glm::vec2& target) noexcept {
    m_target = target;
}

void Camera2D::SetZoom(float zoom) noexcept {
    m_zoom = zoom > 0.0f ? zoom : 1.0f;
}

void Camera2D::SetSize(float width, float height) noexcept {
    m_size = glm::vec2(width, height);
}

void Camera2D::SetBounds(const glm::vec2& min, const glm::vec2& max) noexcept {
    m_minBounds = min;
    m_maxBounds = max;
}

glm::vec2 Camera2D::GetPosition() const noexcept {
    return m_position;
}

glm::vec2 Camera2D::GetTarget() const noexcept {
    return m_target;
}

float Camera2D::GetZoom() const noexcept {
    return m_zoom;
}

glm::vec2 Camera2D::GetSize() const noexcept {
    return m_size;
}

glm::vec2 Camera2D::GetMinBounds() const noexcept {
    return m_minBounds;
}

glm::vec2 Camera2D::GetMaxBounds() const noexcept {
    return m_maxBounds;
}

void Camera2D::Update(float dt, const glm::vec2& playerPosition) {
    const float followSpeed = 6.0f;
    const float deadzone = 64.0f;

    m_target = playerPosition;
    const glm::vec2 delta = m_target - m_position;
    const float distance = glm::length(delta);

    if (distance > deadzone) {
        const float followFactor = std::min(1.0f, dt * followSpeed);
        m_position += delta * followFactor;
    }

    const float halfWidth = m_size.x * 0.5f / m_zoom;
    const float halfHeight = m_size.y * 0.5f / m_zoom;

    m_position.x = std::clamp(m_position.x, m_minBounds.x + halfWidth, m_maxBounds.x - halfWidth);
    m_position.y = std::clamp(m_position.y, m_minBounds.y + halfHeight, m_maxBounds.y - halfHeight);
}

glm::mat4 Camera2D::GetViewMatrix() const noexcept {
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-m_position, 0.0f));
    return glm::scale(view, glm::vec3(m_zoom, m_zoom, 1.0f));
}

glm::mat4 Camera2D::GetProjectionMatrix() const noexcept {
    float halfWidth = m_size.x / 2.0f;
    float halfHeight = m_size.y / 2.0f;
    return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 100.0f);
}

glm::mat4 Camera2D::GetViewProjectionMatrix() const noexcept {
    return GetProjectionMatrix() * GetViewMatrix();
}

} // namespace sclpr::renderer
