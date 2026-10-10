#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_syswm.h>

#include <d3d12.h>
#include <dxgi1_6.h>

#include <wrl/client.h>

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
    bool IsInitialized() const noexcept;

private:
    SDL_Window* m_window{nullptr};

    Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory;
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;

    int m_width{1280};
    int m_height{720};
    bool m_initialized{false};
};

} // namespace sclpr::renderer
