#include "memory/default_memresource.h"
#include "memory/new.h"
#include <iostream>


void* operator new(std::size_t size)
{
    
    
    return std::malloc(size);
}

void* operator new [](size_t _Size)
{

    
    
    return  std::malloc(_Size);
}

void* operator new(size_t _Size, std::nothrow_t const&) noexcept
{
    
    
    return std::malloc(_Size);
}

void* operator new [](size_t _Size, std::nothrow_t const&) noexcept
{
    
    
    return std::malloc(_Size);
}


void operator delete(void* ptr)
{
    if (ptr == nullptr) return;
    
     
    std::free(ptr);
}

void operator delete [](void* _Block)
{
    if (_Block == nullptr) return;
    
    
    std::free(_Block);
}

void operator delete(void* Block, std::nothrow_t const&)
{
    if (Block == nullptr) return;
    
    
    std::free(Block);
}

void operator delete [](void* Block, std::nothrow_t const&)
{
    if (Block == nullptr) return;
    
    
    std::free(Block);
}

void operator delete(void* _Block, size_t _Size)
{
    
    
    if (_Block == nullptr) return;
    std::free(_Block);
}

void operator delete [](void* _Block, size_t _Size)
{
    if (_Block == nullptr) return;
    
    
    std::free(_Block);
}

