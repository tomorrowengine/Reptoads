#pragma once




#ifdef PLATFORM_WINDOWS
#include <d3d12.h>

#include <cstdint>
#include <memory>

class DescriptorAllocatorPage;

class DescriptorAllocation
{
public:
    
    DescriptorAllocation();

    DescriptorAllocation( D3D12_CPU_DESCRIPTOR_HANDLE descriptor, uint32_t numHandles, uint32_t descriptorSize, std::shared_ptr<DescriptorAllocatorPage> page );

    
    ~DescriptorAllocation();

    
    DescriptorAllocation( const DescriptorAllocation& ) = delete;
    DescriptorAllocation& operator=( const DescriptorAllocation& ) = delete;

    
    DescriptorAllocation( DescriptorAllocation&& allocation );
    DescriptorAllocation& operator=( DescriptorAllocation&& other );

    
    bool IsNull() const;

    
    D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle( uint32_t offset = 0 ) const;

    
    uint32_t GetNumHandles() const;

    
    
    std::shared_ptr<DescriptorAllocatorPage> GetDescriptorAllocatorPage() const;

private:
    
    void Free();

    
    D3D12_CPU_DESCRIPTOR_HANDLE m_Descriptor;
    
    uint32_t m_NumHandles;
    
    uint32_t m_DescriptorSize;

    
    std::shared_ptr<DescriptorAllocatorPage> m_Page;
};
#endif
