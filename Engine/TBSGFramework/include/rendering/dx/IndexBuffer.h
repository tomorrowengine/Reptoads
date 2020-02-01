#pragma once




#ifdef PLATFORM_WINDOWS

#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:
    IndexBuffer( const std::wstring& name = L"");
    virtual ~IndexBuffer();

    
    virtual void CreateViews(size_t numElements, size_t elementSize) override;

    size_t GetNumIndicies() const
    {
        return m_NumIndicies;
    }

    DXGI_FORMAT GetIndexFormat() const
    {
        return m_IndexFormat;
    }

    
    D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const
    {
        return m_IndexBufferView;
    }

    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr) const override;

    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr) const override;

protected:

private:
    size_t m_NumIndicies;
    DXGI_FORMAT m_IndexFormat;

    D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
};
#endif
