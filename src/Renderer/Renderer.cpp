#include "Renderer/Renderer.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_syswm.h>

#include <stdexcept>

namespace sclpr::renderer {

Renderer::Renderer(SDL_Window* window) : m_window(window) {}

Renderer::~Renderer() = default;

bool Renderer::IsInitialized() const noexcept {
    return m_initialized;
}

void Renderer::Initialize() {
    if (m_window == nullptr) {
        throw std::runtime_error("Renderer cannot initialize without a valid SDL window.");
    }

#ifdef _WIN32
    SDL_SysWMinfo wmInfo{};
    if (!SDL_GetWindowWMInfo(m_window, &wmInfo)) {
        throw std::runtime_error(std::string("Failed to retrieve native window handle: ") + SDL_GetError());
    }

    HWND hwnd = wmInfo.info.win.window;

    UINT factoryFlags = 0;
    if (FAILED(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_factory)))) {
        throw std::runtime_error("Failed to create DXGI factory.");
    }

    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)))) {
        throw std::runtime_error("Failed to create D3D12 device.");
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    if (FAILED(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)))) {
        throw std::runtime_error("Failed to create D3D12 command queue.");
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = static_cast<UINT>(m_width);
    swapChainDesc.Height = static_cast<UINT>(m_height);
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 3;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
    fullscreenDesc.Windowed = TRUE;

    IDXGISwapChain1* swapChain1 = nullptr;
    if (FAILED(m_factory->CreateSwapChainForHwnd(
            m_commandQueue.Get(),
            hwnd,
            &swapChainDesc,
            &fullscreenDesc,
            nullptr,
            &swapChain1))) {
        throw std::runtime_error("Failed to create D3D12 swap chain.");
    }

    if (FAILED(swapChain1->QueryInterface(IID_PPV_ARGS(&m_swapChain)))) {
        swapChain1->Release();
        throw std::runtime_error("Failed to query IDXGISwapChain3.");
    }

    swapChain1->Release();

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.NumDescriptors = 3;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (FAILED(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)))) {
        throw std::runtime_error("Failed to create RTV descriptor heap.");
    }

    m_initialized = true;
#else
    (void)m_window;
    throw std::runtime_error("DirectX 12 requires a Windows platform for this engine.");
#endif
}

void Renderer::Resize(int width, int height) {
    m_width = width;
    m_height = height;

    if (m_swapChain != nullptr) {
        // Resize logic will be implemented when the frame graph and back buffers are fully managed.
    }
}

void Renderer::BeginFrame() {
    if (!m_initialized) {
        return;
    }

    // Frame start hook. This module will grow into the render graph, clear pass, and command list recording.
}

void Renderer::EndFrame() {
    if (!m_initialized || m_swapChain == nullptr) {
        return;
    }

    m_swapChain->Present(1, 0);
}

} // namespace sclpr::renderer
