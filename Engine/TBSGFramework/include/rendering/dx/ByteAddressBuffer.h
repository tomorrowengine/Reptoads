#pragma once
#include "core/Defines.h"




#ifdef PLATFORM_WINDOWS
#include "Buffer.h"
#include "DescriptorAllocation.h"

#include <d3dx12.h>

class ByteAddressBuffer : public Buffer
{
public:
    ByteAddressBuffer( const std::wstring& name = L"" );
    ByteAddressBuffer( const D3D12_RESOURCE_DESC& resDesc, 
        size_t numElements, size_t elementSize,
        const std::wstring& name = L"");

    size_t GetBufferSize() const
    {
        return m_BufferSize;
    }

    
    virtual void CreateViews( size_t numElements, size_t elementSize ) override;

    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr) const override
    {
		UNUSED(srvDesc);
        return m_SRV.GetDescriptorHandle();
    }

    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr) const override
    {
		UNUSED(uavDesc);
        
        return m_UAV.GetDescriptorHandle();
    }

protected:

private:
    size_t m_BufferSize;

    DescriptorAllocation m_SRV;
    DescriptorAllocation m_UAV;
};
#endif
