#pragma once
#include "memory/memory_resource.h"
#include "memory/details/mem_linked_list.h"
#include <cstdint>

namespace ptl
{
    struct PoolResourceOption;

    class PoolResource : public MemoryResource
    {
        void* DoAllocate(size_t size, const size_t aligment) override;
        bool DoDeallocate(void*& ptr, size_t size, size_t aligment) override;
        bool DoDeallocate(void*& ptr, size_t aligment) override;
        bool DoIsEqual(const MemoryResource& rhs) const override;
        void Init(PoolResourceOption& option);
        uint8_t* m_blob;
        void** _free_list{};
        size_t m_blobSize{};
        size_t m_chunckSize{};
        size_t m_maxChuncks{};
        size_t m_chunckAligment{};
        long m_dmemOffset{};
        size_t m_usedChuncks{};
       
    public:
        PoolResource(PoolResourceOption& option, MemoryResource* upstream = nulloc());
        PoolResource(PoolResourceOption&& option, MemoryResource* upstream = nulloc());
        ~PoolResource();

        PoolResource(const PoolResource& other);
        PoolResource& operator=(const PoolResource& other) = default;
    public:
        size_t id{ 0 };
    };
}
