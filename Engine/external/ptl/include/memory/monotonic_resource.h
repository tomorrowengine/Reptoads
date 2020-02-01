#pragma once
#include "memory/memory_resource.h"
#include <cstdint>

namespace  ptl
{
    class MonotonicResource :public MemoryResource
    {
        void* DoAllocate(size_t size, const size_t aligment) override;
        bool DoDeallocate(void*& ptr, size_t size, size_t aligment) override;
        bool DoDeallocate(void*&, size_t) override;
        bool DoIsEqual(const MemoryResource& rhs) const override;
        void Init(const MemoryResourceOption& option, MemoryResource* upstream);
        size_t m_buffer_size{ 4096 };
        size_t m_freeptr{};
        uint8_t* m_buffer{ nullptr};
        int64_t m_dmemOffset{0};
    public:
		MonotonicResource() = default;
        MonotonicResource(const MemoryResourceOption& option,MemoryResource* upstream = nulloc());
        MonotonicResource(MemoryResourceOption&& option, MemoryResource* upstream = nulloc());
        ~MonotonicResource();
        MonotonicResource(const MonotonicResource& other) = default;
        MonotonicResource& operator=(const MonotonicResource& other) = default;
#ifdef  _DEBUG
        size_t GetFreePtr() { return m_freeptr; }
#endif
    public:
        void Release();
        size_t GetBufferSize()
        {
            return m_buffer_size;
        }
    };
    using LinearResource = MonotonicResource;

}