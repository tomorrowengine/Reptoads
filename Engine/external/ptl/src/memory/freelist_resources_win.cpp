#include "memory/freelist_resources.h"
#include "memory/details/mem_options.h"
#include "memory/details/mem_utils.h"

void* ptl::FreeListResource::DoAllocate(size_t size, const size_t alignment)
{
    assert(size != 0 && alignment != 0);
    FreeBlock* prev_free_block = nullptr;
    FreeBlock* free_block = m_freeBlocks;

    while (free_block != nullptr)
    {
        uintptr_t adjustment = alignForwardAdjustmentWithHeader<Header>(free_block, alignment);
        size_t total_size = size + adjustment;
        if (free_block->size < total_size)
        {
            prev_free_block = free_block;
            free_block = free_block->next;
            continue;
        }
        assert(sizeof(Header) >= sizeof(FreeBlock) && "sizeof(Header) < sizeof(FreeBlock)");
        
        if (free_block->size - total_size <= sizeof(Header))
        {
            
            total_size = free_block->size;

            if (prev_free_block != nullptr)
                prev_free_block->next = free_block->next;
            else
                m_freeBlocks = free_block->next;
        }
        else
        {
            
            FreeBlock* next_block = (FreeBlock*)(add(free_block, total_size));

            next_block->size = free_block->size - total_size;
            next_block->next = free_block->next;

            if (prev_free_block != nullptr)
                prev_free_block->next = next_block;
            else
                m_freeBlocks = next_block;
        }
#pragma warning( push )
#pragma warning( disable  : 4244 )
        const uintptr_t aligned_address = reinterpret_cast<uintptr_t>(free_block) + adjustment;

        auto header = reinterpret_cast<Header*>(aligned_address - sizeof(Header));
        header->size = total_size;
        header->adjustment = adjustment;
        m_usedMemory += static_cast<uint8_t>(total_size);
        m_numAllocations++;
#pragma warning( pop )
        assert(alignForwardAdjustment((void*)aligned_address, alignment) == 0);

        return (void*)aligned_address;
    }
    return nullptr;
}

bool ptl::FreeListResource::DoDeallocate(void*& ptr, size_t size, size_t aligment)
{
    return DoDeallocate(ptr, aligment);
}

bool ptl::FreeListResource::DoDeallocate(void*& ptr, size_t aligment)
{

    assert(ptr != nullptr);
    Header* header = (Header*)subtract(ptr, sizeof(Header));
    uintptr_t block_start = reinterpret_cast<uintptr_t>(ptr) - header->adjustment;
    size_t block_size = header->size;
    uintptr_t block_end = block_start + block_size;
    FreeBlock* prev_free_block = nullptr;
    FreeBlock* free_block = m_freeBlocks;

    while (free_block != nullptr)
    {
        if ((uintptr_t)free_block >= block_end) break;
        prev_free_block = free_block;
        free_block = free_block->next;
    }

    if (prev_free_block == nullptr)
    {
        prev_free_block = (FreeBlock*)block_start;
        prev_free_block->size = block_size;
        prev_free_block->next = m_freeBlocks;
        m_freeBlocks = prev_free_block;
    }
    else if ((uintptr_t)prev_free_block + prev_free_block->size == block_start)
    {
        prev_free_block->size += block_size;
    }
    else
    {
        FreeBlock* temp = (FreeBlock*)block_start;
        temp->size = block_size;
        temp->next = prev_free_block->next;
        prev_free_block->next = temp;
        prev_free_block = temp;
    }

    if (free_block != nullptr && (uintptr_t)free_block == block_end)
    {
        prev_free_block->size += free_block->size;
        prev_free_block->next = free_block->next;
    }

    m_numAllocations--;
    m_usedMemory -= block_size;
    return true;

}

bool ptl::FreeListResource::DoIsEqual(const MemoryResource& rhs) const
{
    return false;
}

ptl::FreeListResource::FreeListResource(MemoryResourceOption&& option, MemoryResource* upstream) :
    MemoryResource(upstream), m_blobSize(option.blobSize), m_align(option.memoryAlingment)
{
    assert(m_blobSize % 16 == 0);
    void* addr = malloc(m_blobSize);
    m_baseAddress = (uint8_t*)(addr);
    m_freeBlocks = (FreeBlock*)(m_baseAddress);
    m_freeBlocks->next = nullptr;
    m_freeBlocks->size = m_blobSize;
}
