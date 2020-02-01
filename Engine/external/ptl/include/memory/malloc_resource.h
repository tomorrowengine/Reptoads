#pragma once
#include "memory/memory_resource.h"


namespace  ptl
{
    class MallocResource : public ptl::MemoryResource
    {
        void* DoAllocate(size_t size, const size_t aligment) override;
        bool DoDeallocate(void*& ptr, size_t size, size_t aligment) override;
        bool DoDeallocate(void*& ptr, size_t aligment) override;
        bool DoIsEqual(const MemoryResource& rhs) const override;
    public:
        MallocResource(MemoryResource* upstream) : MemoryResource(upstream) {};

        MallocResource(const MallocResource& other) = default;
        MallocResource(MallocResource&& other) = default;
        MallocResource& operator=(const MallocResource& other) = default;
        MallocResource& operator=(MallocResource&& other) = default;
    };

    inline ptl::MemoryResource* malloc_resource() noexcept
    {
        static MallocResource instance{ptl::nulloc()};
        return &instance;
    }
}

