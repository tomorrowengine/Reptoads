#ifdef PLATFORM_WINDOWS
#include <rendering/dx/DX12LibPCH.h>

#include <rendering/dx/DescriptorAllocatorPage.h>
#include <rendering/dx/DX12Application.h>

DescriptorAllocatorPage::DescriptorAllocatorPage( D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors )
    : m_HeapType( type )
    , m_NumDescriptorsInHeap( numDescriptors )
{
    auto device = DX12Application::Get().GetDevice();

    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = m_HeapType;
    heapDesc.NumDescriptors = m_NumDescriptorsInHeap;

    ThrowIfFailed( device->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &m_d3d12DescriptorHeap ) ) );

    m_BaseDescriptor = m_d3d12DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    m_DescriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize( m_HeapType );
    m_NumFreeHandles = m_NumDescriptorsInHeap;

    
    AddNewBlock( 0, m_NumFreeHandles );
}

D3D12_DESCRIPTOR_HEAP_TYPE DescriptorAllocatorPage::GetHeapType() const
{
    return m_HeapType;
}

uint32_t DescriptorAllocatorPage::NumFreeHandles() const
{
    return m_NumFreeHandles;
}

bool DescriptorAllocatorPage::HasSpace( uint32_t numDescriptors ) const
{
    return m_FreeListBySize.lower_bound(numDescriptors) != m_FreeListBySize.end();
}

void DescriptorAllocatorPage::AddNewBlock( uint32_t offset, uint32_t numDescriptors )
{
    auto offsetIt = m_FreeListByOffset.emplace( offset, numDescriptors );
    auto sizeIt = m_FreeListBySize.emplace( numDescriptors, offsetIt.first );
    offsetIt.first->second.FreeListBySizeIt = sizeIt;
}

DescriptorAllocation DescriptorAllocatorPage::Allocate( uint32_t numDescriptors )
{
    std::lock_guard<std::mutex> lock( m_AllocationMutex );

    
    
    if ( numDescriptors > m_NumFreeHandles )
    {
        return DescriptorAllocation();
    }

    
    auto smallestBlockIt = m_FreeListBySize.lower_bound( numDescriptors );
    if ( smallestBlockIt == m_FreeListBySize.end() )
    {
        
        return DescriptorAllocation();
    }

    
    auto blockSize = smallestBlockIt->first;

    
    auto offsetIt = smallestBlockIt->second;

    
    auto offset = offsetIt->first;

    
    m_FreeListBySize.erase( smallestBlockIt );
    m_FreeListByOffset.erase( offsetIt );

    
    auto newOffset = offset + numDescriptors;
    auto newSize = blockSize - numDescriptors;

    if ( newSize > 0 )
    {
        
        
        AddNewBlock( newOffset, newSize );
    }

    
    m_NumFreeHandles -= numDescriptors;

    return DescriptorAllocation(
        CD3DX12_CPU_DESCRIPTOR_HANDLE( m_BaseDescriptor, offset, m_DescriptorHandleIncrementSize ),
        numDescriptors, m_DescriptorHandleIncrementSize, shared_from_this() );
}

uint32_t DescriptorAllocatorPage::ComputeOffset( D3D12_CPU_DESCRIPTOR_HANDLE handle )
{
    return static_cast<uint32_t>( handle.ptr - m_BaseDescriptor.ptr ) / m_DescriptorHandleIncrementSize;
}

void DescriptorAllocatorPage::Free( DescriptorAllocation&& descriptor, uint64_t frameNumber )
{
    
    auto offset = ComputeOffset( descriptor.GetDescriptorHandle() );

    std::lock_guard<std::mutex> lock( m_AllocationMutex );

    
    m_StaleDescriptors.emplace( offset, descriptor.GetNumHandles(), frameNumber );
}

void DescriptorAllocatorPage::FreeBlock( uint32_t offset, uint32_t numDescriptors )
{
    
    
    auto nextBlockIt = m_FreeListByOffset.upper_bound( offset );

    
    auto prevBlockIt = nextBlockIt;
    
    if ( prevBlockIt != m_FreeListByOffset.begin() )
    {
        
        --prevBlockIt;
    }
    else
    {
        
        
        prevBlockIt = m_FreeListByOffset.end();
    }

    
    
    
    m_NumFreeHandles += numDescriptors;

    if ( prevBlockIt != m_FreeListByOffset.end() &&
         offset == prevBlockIt->first + prevBlockIt->second.Size )
    {
        
        
        
        
        
        

        
        offset = prevBlockIt->first;
        numDescriptors += prevBlockIt->second.Size;

        
        m_FreeListBySize.erase( prevBlockIt->second.FreeListBySizeIt );
        m_FreeListByOffset.erase( prevBlockIt );
    }

    if ( nextBlockIt != m_FreeListByOffset.end() &&
         offset + numDescriptors == nextBlockIt->first )
    {
        
        
        
        
        

        
        numDescriptors += nextBlockIt->second.Size;

        
        m_FreeListBySize.erase( nextBlockIt->second.FreeListBySizeIt );
        m_FreeListByOffset.erase( nextBlockIt );
    }

    
    AddNewBlock( offset, numDescriptors );
}

void DescriptorAllocatorPage::ReleaseStaleDescriptors( uint64_t frameNumber )
{
    std::lock_guard<std::mutex> lock( m_AllocationMutex );

    while ( !m_StaleDescriptors.empty() && m_StaleDescriptors.front().FrameNumber <= frameNumber )
    {
        auto& staleDescriptor = m_StaleDescriptors.front();

        
        auto offset = staleDescriptor.Offset;
        
        auto numDescriptors = staleDescriptor.Size;

        FreeBlock( offset, numDescriptors );

        m_StaleDescriptors.pop();
    }
}
#endif
