#pragma once
#include <memory>
#include "memory/allocator_impl.h"
namespace ptl {
#ifdef _WIN32
    template<typename T>
    using MemoryAllocator = std::allocator<T>;
#else
    template<typename T>
    using MemoryAllocator = ptl::Allocator<T>;
#endif

    
    extern MemoryAllocator<char> g_GlobalMemoryAllocator;
}
