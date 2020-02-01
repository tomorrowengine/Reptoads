#pragma once





#ifdef PLATFORM_WINDOWS

#include "d3dx12.h"

#include <wrl.h>

#include <vector>

class RootSignature
{
public:
    RootSignature();
    RootSignature(
        const D3D12_ROOT_SIGNATURE_DESC1& rootSignatureDesc, 
        D3D_ROOT_SIGNATURE_VERSION rootSignatureVersion
    );

	RootSignature(const RootSignature& other) = delete;

	RootSignature(RootSignature&& other) noexcept
		: m_RootSignatureDesc{std::move(other.m_RootSignatureDesc)},
		  m_RootSignature{std::move(other.m_RootSignature)}, 
		  
		  m_SamplerTableBitMask{other.m_SamplerTableBitMask},
		  m_DescriptorTableBitMask{other.m_DescriptorTableBitMask}
	{
		other.m_RootSignatureDesc = {};
		std::copy(std::begin(other.m_NumDescriptorsPerTable), std::end(other.m_NumDescriptorsPerTable), std::begin(m_NumDescriptorsPerTable));
		
	}

	RootSignature& operator=(RootSignature other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}

    virtual ~RootSignature();

    void Destroy();

    Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() const
    {
        return m_RootSignature;
    }

    void SetRootSignatureDesc(
        const D3D12_ROOT_SIGNATURE_DESC1& rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION rootSignatureVersion
    );

    const D3D12_ROOT_SIGNATURE_DESC1& GetRootSignatureDesc() const
    {
        return m_RootSignatureDesc;
    }

    uint32_t GetDescriptorTableBitMask(D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType) const;
    uint32_t GetNumDescriptors(uint32_t rootIndex) const;

protected:

private:
    D3D12_ROOT_SIGNATURE_DESC1 m_RootSignatureDesc;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;

    
    
    
    uint32_t m_NumDescriptorsPerTable[32];

    
    
    uint32_t m_SamplerTableBitMask;
    
    
    uint32_t m_DescriptorTableBitMask;
};
#endif
