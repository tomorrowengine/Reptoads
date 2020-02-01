#pragma once
#include "core/Defines.h"





#ifdef PLATFORM_WINDOWS

#include "Buffer.h"

#include "ByteAddressBuffer.h"

class StructuredBuffer : public Buffer
{
public:
    StructuredBuffer( const std::wstring& name = L"" );
    StructuredBuffer( const D3D12_RESOURCE_DESC& resDesc, 
        size_t numElements, size_t elementSize,
        const std::wstring& name = L"");

    
    virtual size_t GetNumElements() const
    {
        return m_NumElements;
    }

    
    virtual size_t GetElementSize() const
    {
        return m_ElementSize;
    }

    
    virtual void CreateViews( size_t numElements, size_t elementSize ) override;

    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr) const
    {
		UNUSED(srvDesc);
        return m_SRV.GetDescriptorHandle();
    }

    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr) const override
    {
		UNUSED(uavDesc);
        
        return m_UAV.GetDescriptorHandle();
    }

    const ByteAddressBuffer& GetCounterBuffer() const
    {
        return m_CounterBuffer;
    }

private:
    size_t m_NumElements;
    size_t m_ElementSize;

    DescriptorAllocation m_SRV;
    DescriptorAllocation m_UAV;

    
    ByteAddressBuffer m_CounterBuffer;
};
#endif
