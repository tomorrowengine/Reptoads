#pragma  once;




#ifdef PLATFORM_WINDOWS
#include "DescriptorAllocation.h"

#include <d3d12.h>

#include <wrl.h>

#include <map>
#include <memory>
#include <mutex>
#include <queue>

class DescriptorAllocatorPage : public std::enable_shared_from_this<DescriptorAllocatorPage>
{
public:
    DescriptorAllocatorPage( D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors );

    D3D12_DESCRIPTOR_HEAP_TYPE GetHeapType() const;

    
    bool HasSpace( uint32_t numDescriptors ) const;

    
    uint32_t NumFreeHandles() const;

    
    DescriptorAllocation Allocate( uint32_t numDescriptors );

    
    void Free( DescriptorAllocation&& descriptorHandle, uint64_t frameNumber );

    
    void ReleaseStaleDescriptors( uint64_t frameNumber );

protected:

    
    uint32_t ComputeOffset( D3D12_CPU_DESCRIPTOR_HANDLE handle );

    
    void AddNewBlock( uint32_t offset, uint32_t numDescriptors );

    
    
    
    void FreeBlock( uint32_t offset, uint32_t numDescriptors );

private:
    
    using OffsetType = uint32_t;
    
    using SizeType = uint32_t;

    struct FreeBlockInfo;
    
    using FreeListByOffset = std::map<OffsetType, FreeBlockInfo>;

    
    
    using FreeListBySize = std::multimap<SizeType, FreeListByOffset::iterator>;

    struct FreeBlockInfo
    {
        FreeBlockInfo( SizeType size )
            : Size( size )
        {}

        SizeType Size;
        FreeListBySize::iterator FreeListBySizeIt;
    };

    struct StaleDescriptorInfo
    {
        StaleDescriptorInfo( OffsetType offset, SizeType size, uint64_t frame )
            : Offset( offset )
            , Size( size )
            , FrameNumber( frame )
        {}

        
        OffsetType Offset;
        
        SizeType Size;
        
        uint64_t FrameNumber;
    };

    
    
    using StaleDescriptorQueue = std::queue<StaleDescriptorInfo>;

    FreeListByOffset m_FreeListByOffset;
    FreeListBySize m_FreeListBySize;
    StaleDescriptorQueue m_StaleDescriptors;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_d3d12DescriptorHeap;
    D3D12_DESCRIPTOR_HEAP_TYPE m_HeapType;
    CD3DX12_CPU_DESCRIPTOR_HANDLE m_BaseDescriptor;
    uint32_t m_DescriptorHandleIncrementSize;
    uint32_t m_NumDescriptorsInHeap;
    uint32_t m_NumFreeHandles;

    std::mutex m_AllocationMutex;
};
#endif
