#pragma once
#include "memory/memory_resource.h"
#include <cstdint>
namespace ptl
{
    class FreeListResource : public MemoryResource
    {
        void* DoAllocate(size_t size, const size_t aligment) override;
        bool DoDeallocate(void*& ptr, size_t size, size_t aligment) override;
        bool DoDeallocate(void*& ptr, size_t aligment) override;
        bool DoIsEqual(const MemoryResource& rhs) const override;

        struct Header
        {
            size_t size; 
            std::uint8_t adjustment;
        };

        struct FreeBlock
        {
            size_t size; 
            FreeBlock* next;
        };
        size_t m_blobSize{};
        size_t m_align{};
        int64_t m_dmemOffset{ 0 };
        uint8_t* m_baseAddress{ nullptr };
        FreeBlock* m_freeBlocks{nullptr};

        size_t m_usedMemory{};
        size_t m_numAllocations{};
    public:
        explicit FreeListResource(MemoryResourceOption&& option, MemoryResource* upstream = ptl::nulloc());
    };
}
