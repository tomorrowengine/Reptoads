#pragma once




#ifdef PLATFORM_WINDOWS
#include "DescriptorAllocation.h"

#include "d3dx12.h"

#include <cstdint>
#include <mutex>
#include <memory>
#include <set>
#include <vector>

class DescriptorAllocatorPage;

class DescriptorAllocator
{
public:
    DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptorsPerHeap = 256);
    virtual ~DescriptorAllocator();

    
    DescriptorAllocation Allocate(uint32_t numDescriptors = 1);

    
    void ReleaseStaleDescriptors( uint64_t frameNumber );

private:
    using DescriptorHeapPool = std::vector< std::shared_ptr<DescriptorAllocatorPage> >;

    
    std::shared_ptr<DescriptorAllocatorPage> CreateAllocatorPage();

    D3D12_DESCRIPTOR_HEAP_TYPE m_HeapType;
    uint32_t m_NumDescriptorsPerHeap;

    DescriptorHeapPool m_HeapPool;
    
    std::set<size_t> m_AvailableHeaps;

    std::mutex m_AllocationMutex;
};
#endif
