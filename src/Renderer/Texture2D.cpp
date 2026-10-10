#include "Renderer/Texture2D.h"

#include <stdexcept>

namespace sclpr::renderer {

Texture2D::Texture2D(ID3D12Device* device, int width, int height, const void* data)
    : m_width(width), m_height(height) {
    if (device == nullptr) {
        throw std::runtime_error("Cannot create texture with null device.");
    }

    // For now, texture creation is stubbed.
    // Full implementation will include texture upload heaps and command lists.
}

Texture2D::~Texture2D() = default;

ID3D12Resource* Texture2D::GetResource() const noexcept {
    return m_texture.Get();
}

} // namespace sclpr::renderer
