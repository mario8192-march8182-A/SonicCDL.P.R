#include "Renderer/Camera2D.h"

namespace sclpr::renderer {

Camera2D::Camera2D(float width, float height)
    : m_size(width, height) {}

void Camera2D::SetPosition(const glm::vec2& position) noexcept {
    m_position = position;
}

void Camera2D::SetZoom(float zoom) noexcept {
    m_zoom = zoom > 0.0f ? zoom : 1.0f;
}

void Camera2D::SetSize(float width, float height) noexcept {
    m_size = glm::vec2(width, height);
}

glm::vec2 Camera2D::GetPosition() const noexcept {
    return m_position;
}

float Camera2D::GetZoom() const noexcept {
    return m_zoom;
}

glm::vec2 Camera2D::GetSize() const noexcept {
    return m_size;
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
