#include "memory/pool_resource.h"
#include "memory/details/mem_utils.h"
#include "memory/details/mem_options.h"

namespace ptl
{
    void* PoolResource::DoAllocate(size_t size, const size_t alignment)
    {
        if (m_maxChuncks <= m_usedChuncks)
        {
            return nullptr;
        };
        if (size >= m_chunckSize)
        {
            return nullptr;
        };
        if (alignment > m_chunckAligment)
        {
            return nullptr;
        }
        void* p = _free_list;
        _free_list = (void**)(*_free_list);
        ++m_usedChuncks;
        return p;
    }

    bool PoolResource::DoDeallocate(void*& ptr, size_t , size_t )
    {
        *((void**)ptr) = _free_list;
        _free_list = (void**)ptr;
        --m_usedChuncks;
        return true;
    }

    bool PoolResource::DoDeallocate(void*& ptr, size_t)
    {
        *((void**)ptr) = _free_list;
        _free_list = (void**)ptr;
        --m_usedChuncks;
        return true;
    }

    bool PoolResource::DoIsEqual(const MemoryResource& rhs) const
    {
        return this == &rhs;
    }

    PoolResource::PoolResource(PoolResourceOption& option, MemoryResource* upstream) : MemoryResource(upstream),
                                                                                       m_chunckSize(option.chunckSize),
                                                                                       m_blobSize(option.blobSize),
                                                                                       m_chunckAligment(
                                                                                           option.memoryAlingment)
    {
        Init(option);
    }
    PoolResource::PoolResource(PoolResourceOption&& option, MemoryResource* upstream) : MemoryResource(upstream),
                                                                                        m_chunckSize(option.chunckSize),
                                                                                        m_blobSize(option.blobSize),
                                                                                        m_chunckAligment(
                                                                                            option.memoryAlingment)
    {
        Init(option);
    }

    void PoolResource::Init(PoolResourceOption& option)
    {
        m_blob = static_cast<uint8_t*>(malloc(m_blobSize));
        auto space = m_blobSize;
        auto adjustment = align_offset(m_chunckAligment,m_chunckSize,m_blob,space);

        _free_list = (void**) add(m_blob, adjustment);
        m_maxChuncks = (m_blobSize - adjustment) / m_chunckSize;
        void** p = _free_list;
        
        for (size_t i = 0; i < m_maxChuncks - 1; i++)
        {
            *p = add(p, m_chunckSize);
            p = (void**)*p;
        }
        *p = nullptr;
    }

    PoolResource::~PoolResource()
    {
        free(m_blob);
    }
}
