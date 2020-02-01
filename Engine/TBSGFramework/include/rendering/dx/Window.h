#pragma once




#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_5.h>

#include <rendering/dx/HighResolutionClock.h>
#include <rendering/dx/RenderTarget.h>
#include <rendering/dx/Texture.h>
#include "Events.h"

#include <memory>

class Texture;

class Window : public std::enable_shared_from_this<Window>
{
public:
    
    static const UINT BufferCount = 3;

	
	std::function< void (UpdateEventArgs& e)> OnUpdateEvent;
	std::function< void (RenderEventArgs& e)> OnRenderEvent;

	
	std::function<void(KeyEventArgs& e)> OnKeyPressedEvent;
	
	std::function<void(KeyEventArgs& e)> OnKeyReleasedEvent;

	
	std::function<void(MouseMotionEventArgs& e)> OnMouseMovedEvent;
	
	std::function<void(MouseButtonEventArgs& e)> OnMouseButtonPressedEvent;
	
	std::function<void (MouseButtonEventArgs& e)> OnMouseButtonReleasedEvent;
	
	std::function<void (MouseWheelEventArgs& e)> OnMouseWheelEvent;

	
	std::function<void (ResizeEventArgs& e)> OnResizeEvent;

    
    HWND GetWindowHandle() const;

    
    void Initialize();

    
    void Destroy();

    const std::wstring& GetWindowName() const;

    int GetClientWidth() const;
    int GetClientHeight() const;

    
    bool IsVSync() const;
    void SetVSync(bool vSync);
    void ToggleVSync();

    
    bool IsFullScreen() const;

    
    void SetFullscreen(bool fullscreen);
    void ToggleFullscreen();

    
    void Show();

    
    void Hide();

    
    const RenderTarget& GetRenderTarget() const;

    
    UINT Present( const Texture& texture = Texture() );

protected:
    
    friend LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    
    friend class DX12Application;
	friend class DX12Renderer;
    
    

    Window() = delete;
    Window(HWND hWnd, const std::wstring& windowName, int clientWidth, int clientHeight, bool vSync );
    virtual ~Window();

    
    
    

	
    
    virtual void OnUpdate(UpdateEventArgs& e);
    virtual void OnRender(RenderEventArgs& e);

    
    virtual void OnKeyPressed(KeyEventArgs& e);
    
    virtual void OnKeyReleased(KeyEventArgs& e);

    
    virtual void OnMouseMoved(MouseMotionEventArgs& e);
    
    virtual void OnMouseButtonPressed(MouseButtonEventArgs& e);
    
    virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);
    
    virtual void OnMouseWheel(MouseWheelEventArgs& e);

    
    virtual void OnResize(ResizeEventArgs& e);
	

    
    Microsoft::WRL::ComPtr<IDXGISwapChain4> CreateSwapChain();

    
    void UpdateRenderTargetViews();

private:
    
    Window(const Window& copy) = delete;
    Window& operator=(const Window& other) = delete;

    HWND m_hWnd;

    std::wstring m_WindowName;
    
    int m_ClientWidth;
    int m_ClientHeight;
    bool m_VSync;
    bool m_Fullscreen;

    HighResolutionClock m_UpdateClock;
    HighResolutionClock m_RenderClock;

    UINT64 m_FenceValues[BufferCount];
    uint64_t m_FrameValues[BufferCount];

    

    Microsoft::WRL::ComPtr<IDXGISwapChain4> m_dxgiSwapChain;
    Texture m_BackBufferTextures[BufferCount];
    
    mutable RenderTarget m_RenderTarget;

    UINT m_CurrentBackBufferIndex;

    RECT m_WindowRect;
    bool m_IsTearingSupported;

    int m_PreviousMouseX;
    int m_PreviousMouseY;


};
#endif
