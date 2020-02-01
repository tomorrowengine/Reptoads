#pragma once



#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> 



#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif



#if defined(CreateWindow)
#undef CreateWindow
#endif


#include <wrl.h>
using namespace Microsoft::WRL;


#include "d3dx12.h"
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTex.h>

using namespace DirectX;


#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <experimental/filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <unordered_map>
#include <thread>
#include <vector>

namespace fs = std::experimental::filesystem;


#include <rendering/dx/Helpers.h>
