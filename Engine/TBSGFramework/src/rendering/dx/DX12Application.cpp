#include "IApplication.h"
#if defined(PLATFORM_WINDOWS)
#include <rendering/dx/DX12LibPCH.h>
#include <rendering/dx/DX12Application.h>


#include <rendering/dx/CommandQueue.h>

#include <rendering/dx/DescriptorAllocator.h>
#include "core/Defines.h"
#include <iostream>
#include <Windows.h>
#include <rendering/dx/Window.h>
#include "resource.h"

constexpr wchar_t WINDOW_CLASS_NAME[] = L"DX12RenderWindowClass";

using WindowPtr = std::shared_ptr<Window>;
using WindowMap = std::map< HWND, WindowPtr >;
using WindowNameMap = std::map< std::wstring, WindowPtr >;

static DX12Application* gs_pSingelton = nullptr;
static WindowMap gs_Windows;
static WindowNameMap gs_WindowByName;

uint64_t DX12Application::ms_FrameCount = 0;

extern LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );




struct MakeWindow : public Window 
{
    MakeWindow(HWND hWnd, const std::wstring& windowName, int clientWidth, int clientHeight, bool vSync)
        : Window(hWnd, windowName, clientWidth, clientHeight, vSync)
    {}
};

DX12Application::DX12Application(HINSTANCE hInst)
    : m_hInstance(hInst)
    , m_TearingSupported(false)
{
    
    
    
    
    SetThreadDpiAwarenessContext( DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 );

    WNDCLASSEXW wndClass = { 0 };

    wndClass.cbSize = sizeof( WNDCLASSEX );
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = &WndProc;
    wndClass.hInstance = m_hInstance;
    wndClass.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wndClass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(MAINICON));
    wndClass.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = WINDOW_CLASS_NAME;
	wndClass.hIconSm = NULL; 

    if ( !RegisterClassExW( &wndClass ) )
    {
        MessageBoxA( NULL, "Unable to register the window class.", "Error", MB_OK | MB_ICONERROR );
    }
}

void DX12Application::Initialize()
{
#if defined(_DEBUG) && !defined(RENDERING_DISABLE_DEBUGLAYER)
    
    
    
    ComPtr<ID3D12Debug1> debugInterface;
    ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
    debugInterface->EnableDebugLayer();
    
    
    
#endif

    auto dxgiAdapter = GetAdapter(false);
    if ( !dxgiAdapter )
    {
        
        dxgiAdapter = GetAdapter( true );
    }

    if (dxgiAdapter)
    {
		DXGI_ADAPTER_DESC1 description;
		dxgiAdapter->GetDesc1(&description);
		printf("Using device: %ls\n", description.Description);
        m_d3d12Device = CreateDevice(dxgiAdapter);
    }
    else
    {
        throw std::exception( "DXGI adapter enumeration failed." );
    }
    
    m_DirectCommandQueue = std::make_shared<CommandQueue>(D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_ComputeCommandQueue = std::make_shared<CommandQueue>(D3D12_COMMAND_LIST_TYPE_COMPUTE);
    m_CopyCommandQueue = std::make_shared<CommandQueue>(D3D12_COMMAND_LIST_TYPE_COPY);

    m_TearingSupported = CheckTearingSupport();

    
    for (int i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
    {
        m_DescriptorAllocators[i] = std::make_unique<DescriptorAllocator>(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i));
    }

    
    ms_FrameCount = 0;
}

void DX12Application::Create(HINSTANCE hInst)
{
    if (!gs_pSingelton)
    {
		CoInitialize(nullptr);
        gs_pSingelton = new DX12Application(hInst);
        gs_pSingelton->Initialize();
    }
}

DX12Application& DX12Application::Get()
{
    assert(gs_pSingelton);
    return *gs_pSingelton;
}

void DX12Application::Destroy()
{
    if (gs_pSingelton)
    {
        assert( gs_Windows.empty() && gs_WindowByName.empty() && 
            "All windows should be destroyed before destroying the application instance.");

        delete gs_pSingelton;
        gs_pSingelton = nullptr;
    }
}

DX12Application::~DX12Application()
{
    Flush();
}

Microsoft::WRL::ComPtr<IDXGIAdapter4> DX12Application::GetAdapter(bool bUseWarp)
{
    ComPtr<IDXGIFactory4> dxgiFactory;
    UINT createFactoryFlags = 0;
#if defined(_DEBUG)
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

    ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

    ComPtr<IDXGIAdapter1> dxgiAdapter1;
    ComPtr<IDXGIAdapter4> dxgiAdapter4;

    if (bUseWarp)
    {
        ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
        ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
    }
    else
    {
        SIZE_T maxDedicatedVideoMemory = 0;
        for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
            dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

            
            
            
            if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
                SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
                    D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
                dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
            {
                maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
                ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
            }
        }
    }

    return dxgiAdapter4;
}
Microsoft::WRL::ComPtr<ID3D12Device2> DX12Application::CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter)
{
    ComPtr<ID3D12Device2> d3d12Device2;
    ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)));


    
#if defined(_DEBUG)
    ComPtr<ID3D12InfoQueue> pInfoQueue;
    if (SUCCEEDED(d3d12Device2.As(&pInfoQueue)))
    {
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

        
        

        
        D3D12_MESSAGE_SEVERITY Severities[] =
        {
            D3D12_MESSAGE_SEVERITY_INFO
        };

        
        D3D12_MESSAGE_ID DenyIds[] = {
			D3D12_MESSAGE_ID_COPY_DESCRIPTORS_INVALID_RANGES,				
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       
        };

        D3D12_INFO_QUEUE_FILTER NewFilter = {};
        
        
        NewFilter.DenyList.NumSeverities = _countof(Severities);
        NewFilter.DenyList.pSeverityList = Severities;
        NewFilter.DenyList.NumIDs = _countof(DenyIds);
        NewFilter.DenyList.pIDList = DenyIds;

        ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));
    }
#endif

    return d3d12Device2;
}

bool DX12Application::CheckTearingSupport()
{
    BOOL allowTearing = FALSE;

    
    
    
    
    ComPtr<IDXGIFactory4> factory4;
    if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
    {
        ComPtr<IDXGIFactory5> factory5;
        if (SUCCEEDED(factory4.As(&factory5)))
        {
            factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,
                &allowTearing, sizeof(allowTearing));
        }
    }

    return allowTearing == TRUE;
}

bool DX12Application::IsTearingSupported() const
{
    return m_TearingSupported;
}

DXGI_SAMPLE_DESC DX12Application::GetMultisampleQualityLevels( DXGI_FORMAT format, UINT numSamples, D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS flags ) const
{
    DXGI_SAMPLE_DESC sampleDesc = { 1, 0 };

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels;
    qualityLevels.Format = format;
    qualityLevels.SampleCount = 1;
    qualityLevels.Flags = flags;
    qualityLevels.NumQualityLevels = 0;

    while ( qualityLevels.SampleCount <= numSamples && SUCCEEDED( m_d3d12Device->CheckFeatureSupport( D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevels, sizeof( D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS ) ) ) && qualityLevels.NumQualityLevels > 0 )
    {
        
        sampleDesc.Count = qualityLevels.SampleCount;
        sampleDesc.Quality = qualityLevels.NumQualityLevels - 1;

        
        qualityLevels.SampleCount *= 2;
    }

    return sampleDesc;
}


std::shared_ptr<Window> DX12Application::CreateRenderWindow(const std::wstring& windowName, int clientWidth, int clientHeight, bool vSync )
{
    
    WindowNameMap::iterator windowIter = gs_WindowByName.find(windowName);
    if (windowIter != gs_WindowByName.end())
    {
        return windowIter->second;
    }

    RECT windowRect = { 0, 0, clientWidth, clientHeight };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowW(WINDOW_CLASS_NAME, windowName.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, nullptr, m_hInstance, nullptr);

    if (!hWnd)
    {
        MessageBoxA(NULL, "Could not create the render window.", "Error", MB_OK | MB_ICONERROR);
        return nullptr;
    }

    WindowPtr pWindow = std::make_shared<MakeWindow>(hWnd, windowName, clientWidth, clientHeight, vSync);
    pWindow->Initialize();

    gs_Windows.insert(WindowMap::value_type(hWnd, pWindow));
    gs_WindowByName.insert(WindowNameMap::value_type(windowName, pWindow));

    return pWindow;
}

void DX12Application::DestroyWindow(std::shared_ptr<Window> window)
{
    if (window) window->Destroy();
}

void DX12Application::DestroyWindow(const std::wstring& windowName)
{
    WindowPtr pWindow = GetWindowByName(windowName);
    if ( pWindow )
    {
        DestroyWindow(pWindow);
    }
}

std::shared_ptr<Window> DX12Application::GetWindowByName(const std::wstring& windowName)
{
    std::shared_ptr<Window> window;
    WindowNameMap::iterator iter = gs_WindowByName.find(windowName);
    if (iter != gs_WindowByName.end())
    {
        window = iter->second;
    }

    return window;
}

void DX12Application::PollWindowEvents()
{
	EmptyFunction();
	MSG msg{};
	while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_CHAR)
			EmptyFunction(); 
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_PAINT)
			break;
		
	}
	EmptyFunction();
}




void DX12Application::Quit(int exitCode)
{
    PostQuitMessage(exitCode);
	tbsg::IApplication::m_ShouldStop = true;
}

Microsoft::WRL::ComPtr<ID3D12Device2> DX12Application::GetDevice() const
{
    return m_d3d12Device;
}

std::shared_ptr<CommandQueue> DX12Application::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type) const
{
    std::shared_ptr<CommandQueue> commandQueue;
    switch (type)
    {
    case D3D12_COMMAND_LIST_TYPE_DIRECT:
        commandQueue = m_DirectCommandQueue;
        break;
    case D3D12_COMMAND_LIST_TYPE_COMPUTE:
        commandQueue = m_ComputeCommandQueue;
        break;
    case D3D12_COMMAND_LIST_TYPE_COPY:
        commandQueue = m_CopyCommandQueue;
        break;
    default:
        assert(false && "Invalid command queue type.");
    }

    return commandQueue;
}

void DX12Application::Flush()
{
    m_DirectCommandQueue->Flush();
    m_ComputeCommandQueue->Flush();
    m_CopyCommandQueue->Flush();
}

DescriptorAllocation DX12Application::AllocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors)
{
    return m_DescriptorAllocators[type]->Allocate(numDescriptors);
}

void DX12Application::ReleaseStaleDescriptors( uint64_t finishedFrame )
{
    for ( int i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i )
    {
        m_DescriptorAllocators[i]->ReleaseStaleDescriptors( finishedFrame );
    }
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DX12Application::CreateDescriptorHeap(UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = type;
    desc.NumDescriptors = numDescriptors;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NodeMask = 0;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
    ThrowIfFailed(m_d3d12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

    return descriptorHeap;
}

UINT DX12Application::GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type) const
{
    return m_d3d12Device->GetDescriptorHandleIncrementSize(type);
}



static void RemoveWindow(HWND hWnd)
{
    WindowMap::iterator windowIter = gs_Windows.find(hWnd);
    if (windowIter != gs_Windows.end())
    {
        WindowPtr pWindow = windowIter->second;
        gs_WindowByName.erase(pWindow->GetWindowName());
        gs_Windows.erase(windowIter);
    }
}


MouseButtonEventArgs::MouseButton DecodeMouseButton(UINT messageID)
{
    MouseButtonEventArgs::MouseButton mouseButton = MouseButtonEventArgs::None;
    switch (messageID)
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
    {
        mouseButton = MouseButtonEventArgs::Left;
    }
    break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
    {
        mouseButton = MouseButtonEventArgs::Right;
    }
    break;
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK:
    {
        mouseButton = MouseButtonEventArgs::Middel;
    }
    break;
    }

    return mouseButton;
}

#include "core/Input.h"

extern LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (tbsg::Input::m_Instance != nullptr) {
		tbsg::Input::m_Instance->OnWndProc(message, wParam, lParam);
	}
    
    
    
    
    

    WindowPtr pWindow;
    {
        WindowMap::iterator iter = gs_Windows.find(hwnd);
        if (iter != gs_Windows.end())
        {
            pWindow = iter->second;
        }
    }

    if ( pWindow )
    {
        switch (message)
        {
        case WM_PAINT:
        {
            ++DX12Application::ms_FrameCount;

            
            UpdateEventArgs updateEventArgs(0.0f, 0.0f, DX12Application::ms_FrameCount);
            pWindow->OnUpdate(updateEventArgs);
            RenderEventArgs renderEventArgs(0.0f, 0.0f, DX12Application::ms_FrameCount);
            
            pWindow->OnRender(renderEventArgs);
        }
        break;
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            MSG charMsg;
            
            unsigned int c = 0;
            
            
            
            if (PeekMessage(&charMsg, hwnd, 0, 0, PM_NOREMOVE ) && charMsg.message == WM_CHAR)
            {
                GetMessage(&charMsg, hwnd, 0, 0);
                c = static_cast<unsigned int>( charMsg.wParam );

				if (charMsg.wParam > 0 && charMsg.wParam < 0x10000)
				{
					tbsg::Input::m_Instance->OnWndProc(charMsg.message, charMsg.wParam, charMsg.lParam);
				}
            }
            bool shift = ( GetAsyncKeyState(VK_SHIFT) & 0x8000 ) != 0;
            bool control = ( GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
            bool alt = ( GetAsyncKeyState(VK_MENU) & 0x8000 ) != 0;
            KeyCode::Key key = (KeyCode::Key)wParam;
            
            KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Pressed, shift, control, alt);
        pWindow->OnKeyPressed(keyEventArgs);
        }
        break;
        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
            bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
            bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
            KeyCode::Key key = (KeyCode::Key)wParam;
            unsigned int c = 0;
            unsigned int scanCode = (lParam & 0x00FF0000) >> 16;

            
            
            
            unsigned char keyboardState[256];
            GetKeyboardState(keyboardState);
            wchar_t translatedCharacters[4];
            if (int result = ToUnicodeEx(static_cast<UINT>( wParam ), scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0)
            {
                c = translatedCharacters[0];
            }

            KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Released, shift, control, alt);
        pWindow->OnKeyReleased(keyEventArgs);
        }
        break;
        
        
        
        case WM_SYSCHAR:
            break;
        case WM_MOUSEMOVE:
        {
            bool lButton = (wParam & MK_LBUTTON) != 0;
            bool rButton = (wParam & MK_RBUTTON) != 0;
            bool mButton = (wParam & MK_MBUTTON) != 0;
            bool shift = (wParam & MK_SHIFT) != 0;
            bool control = (wParam & MK_CONTROL) != 0;

            int x = ((int)(short)LOWORD(lParam));
            int y = ((int)(short)HIWORD(lParam));

            MouseMotionEventArgs mouseMotionEventArgs(lButton, mButton, rButton, control, shift, x, y);
         pWindow->OnMouseMoved(mouseMotionEventArgs);
        }
        break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        {
            bool lButton = (wParam & MK_LBUTTON) != 0;
            bool rButton = (wParam & MK_RBUTTON) != 0;
            bool mButton = (wParam & MK_MBUTTON) != 0;
            bool shift = (wParam & MK_SHIFT) != 0;
            bool control = (wParam & MK_CONTROL) != 0;

            int x = ((int)(short)LOWORD(lParam));
            int y = ((int)(short)HIWORD(lParam));

            MouseButtonEventArgs mouseButtonEventArgs(DecodeMouseButton(message), MouseButtonEventArgs::Pressed, lButton, mButton, rButton, control, shift, x, y);
      pWindow->OnMouseButtonPressed(mouseButtonEventArgs);
        }
        break;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            bool lButton = (wParam & MK_LBUTTON) != 0;
            bool rButton = (wParam & MK_RBUTTON) != 0;
            bool mButton = (wParam & MK_MBUTTON) != 0;
            bool shift = (wParam & MK_SHIFT) != 0;
            bool control = (wParam & MK_CONTROL) != 0;

            int x = ((int)(short)LOWORD(lParam));
            int y = ((int)(short)HIWORD(lParam));

            MouseButtonEventArgs mouseButtonEventArgs(DecodeMouseButton(message), MouseButtonEventArgs::Released, lButton, mButton, rButton, control, shift, x, y);
       pWindow->OnMouseButtonReleased(mouseButtonEventArgs);
        }
        break;
        case WM_MOUSEWHEEL:
        {
            
            
            
            float zDelta = ((int)(short)HIWORD(wParam)) / (float)WHEEL_DELTA;
            short keyStates = (short)LOWORD(wParam);

            bool lButton = (keyStates & MK_LBUTTON) != 0;
            bool rButton = (keyStates & MK_RBUTTON) != 0;
            bool mButton = (keyStates & MK_MBUTTON) != 0;
            bool shift = (keyStates & MK_SHIFT) != 0;
            bool control = (keyStates & MK_CONTROL) != 0;

            int x = ((int)(short)LOWORD(lParam));
            int y = ((int)(short)HIWORD(lParam));

            
            POINT clientToScreenPoint;
            clientToScreenPoint.x = x;
            clientToScreenPoint.y = y;
            ScreenToClient(hwnd, &clientToScreenPoint);

            MouseWheelEventArgs mouseWheelEventArgs(zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y);
         pWindow->OnMouseWheel(mouseWheelEventArgs);
        }
        break;
        case WM_SIZE:
        {
            int width = ((int)(short)LOWORD(lParam));
            int height = ((int)(short)HIWORD(lParam));

            ResizeEventArgs resizeEventArgs(width, height);
            pWindow->OnResize(resizeEventArgs);
        }
        break;
        case WM_DESTROY:
        {
            
            
            RemoveWindow(hwnd);

            if (gs_Windows.empty())
            {

                
				DX12Application::Get().Quit(0);
				
            }
        }
        break;
        default:
            return DefWindowProcW(hwnd, message, wParam, lParam);
        }
    }
    else
    {
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }

    return 0;
}
#endif
