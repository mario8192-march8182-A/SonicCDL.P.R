#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <glm/glm.hpp>

#include <string>
#include <memory>

namespace sclpr::renderer {

class Texture2D {
public:
    Texture2D(ID3D12Device* device, int width, int height, const void* data = nullptr);
    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    ID3D12Resource* GetResource() const noexcept;
    int GetWidth() const noexcept { return m_width; }
    int GetHeight() const noexcept { return m_height; }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
    int m_width{0};
    int m_height{0};
};

} // namespace sclpr::renderer
