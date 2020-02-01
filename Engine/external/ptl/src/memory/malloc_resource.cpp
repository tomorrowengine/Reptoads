#include "memory/malloc_resource.h"
#include "memory/memory_resource.h"
#include <iostream>

void* ptl::MallocResource::DoAllocate(size_t size, const size_t )
{
    return std::malloc(size);
}

bool ptl::MallocResource::DoDeallocate(void*& ptr, size_t , size_t )
{
    
    std::free(ptr);
    
    return true;
}

bool ptl::MallocResource::DoDeallocate(void*& ptr, size_t )
{
    
    
    std::free(ptr);
    
    return true;
}

bool ptl::MallocResource::DoIsEqual(const MemoryResource&) const
{
    return true;
}
