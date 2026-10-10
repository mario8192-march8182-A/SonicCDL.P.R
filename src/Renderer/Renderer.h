#pragma once

#include "Renderer/Camera2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/Texture2D.h"

#include <SDL3/SDL.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include <memory>
#include <vector>

namespace sclpr::renderer {

class Renderer {
public:
    explicit Renderer(SDL_Window* window);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void Initialize();
    void Resize(int width, int height);
    void BeginFrame();
    void EndFrame();
    void Clear(float r, float g, float b, float a = 1.0f);
    bool IsInitialized() const noexcept;

    // Rendering interface
    void DrawSprite(const Sprite& sprite);
    void DrawRectangle(float x, float y, float width, float height, const glm::vec4& color);

    // Camera access
    Camera2D* GetCamera() noexcept { return m_camera.get(); }
    const Camera2D* GetCamera() const noexcept { return m_camera.get(); }

    // Device access for resource creation
    ID3D12Device* GetDevice() noexcept { return m_device.Get(); }

private:
    SDL_Window* m_window{nullptr};

    Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory;
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

    std::unique_ptr<Camera2D> m_camera;

    int m_width{1280};
    int m_height{720};
    bool m_initialized{false};
    UINT64 m_fenceValue{0};
};

} // namespace sclpr::renderer
