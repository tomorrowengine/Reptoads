#pragma once
#include "monotonic_resource.h"
#include "memory/pool_resource.h"
#include "memory/details/mem_defines.h"
#include "memory/details/mem_options.h"
#include "malloc_resource.h"


namespace ptl
{
    class DefaultMemoryResource
    {
        LinearResource lmr;
		MemoryResource* allocs[mem::numberOfPools + 1]{};
        DefaultMemoryResource();
    public:
        static MemoryResource* get_default_allocator();
		MemoryResource* GetPool(unsigned int index)
        {
            return allocs[index];
        }

        static MonotonicResource* get_default_monotonic();
    };





}
