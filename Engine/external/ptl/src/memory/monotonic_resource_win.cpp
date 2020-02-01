#include "memory/monotonic_resource.h"
#include "memory/details/mem_utils.h"
#include "memory/details/mem_options.h"

#include <cstdlib>
#include <cstring>

namespace ptl
{
    void* MonotonicResource::DoAllocate(size_t size, const size_t aligment)
    {
        const size_t offset = alignTo(m_freeptr, aligment);
        const size_t newSize = offset + size;
        if (newSize > m_buffer_size)
            return nullptr;
        m_freeptr = newSize;
        return m_buffer + newSize;
    }

    bool MonotonicResource::DoDeallocate(void*& ptr, size_t size, size_t )
    {
        memset(ptr, 0xCC, size);
        return true; 
    }

    bool MonotonicResource::DoDeallocate(void*& ptr, size_t size)
    {
        memset(ptr, 0xCC, size);
        ptr = nullptr;
        return true; 
    }

    bool MonotonicResource::DoIsEqual(const MemoryResource& rhs) const
    {
        return this == &rhs;
    }

    MonotonicResource::MonotonicResource(const MemoryResourceOption& option, MemoryResource* upstream):MemoryResource(upstream), m_buffer_size(option.blobSize)
    {
        m_buffer = static_cast<uint8_t*>(malloc(option.blobSize));
        m_freeptr = 0;
    }
    MonotonicResource::MonotonicResource(MemoryResourceOption&& option, MemoryResource* upstream) : MemoryResource(upstream), m_buffer_size(option.blobSize)
    {
        m_buffer = static_cast<uint8_t*>(malloc(option.blobSize));
        m_freeptr = 0;
    }
    

    MonotonicResource::~MonotonicResource()
    {
        free(m_buffer);
    }

    void MonotonicResource::Release()
    {
#ifdef _DEBUG
        m_numberOfdealloc = 0;
        m_numberOfalloc = 0;
#endif
        m_freeptr = 0;
        memset(m_buffer, 0xCC, m_buffer_size);
    }
}
