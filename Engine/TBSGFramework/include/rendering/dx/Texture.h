#pragma once





#ifdef PLATFORM_WINDOWS

#include "Resource.h"
#include "DescriptorAllocation.h"
#include "TextureUsage.h"

#include "d3dx12.h"
#include <glm/vec2.hpp>

#include <mutex>
#include <unordered_map>
 
class Texture : public Resource
{
public:
    explicit Texture(TextureUsage textureUsage = TextureUsage::Albedo, 
                      const std::wstring& name = L"" );
    explicit Texture( const D3D12_RESOURCE_DESC& resourceDesc, 
                      const D3D12_CLEAR_VALUE* clearValue = nullptr,
                      TextureUsage textureUsage = TextureUsage::Albedo,
                      const std::wstring& name = L"" );
    explicit Texture( Microsoft::WRL::ComPtr<ID3D12Resource> resource,
                      TextureUsage textureUsage = TextureUsage::Albedo,
                      const std::wstring& name = L"");

    Texture(const Texture& copy);
    Texture( Texture&& copy );
	glm::vec2 GetUVMin() { return { 0.0f,0.0f }; }

    Texture& operator=(const Texture& other);
    Texture& operator=(Texture&& other);

    virtual ~Texture();

    TextureUsage GetTextureUsage() const
    {
        return m_TextureUsage;
    }

    void SetTextureUsage( TextureUsage textureUsage )
    {
        m_TextureUsage = textureUsage;
    }

    
    void Resize(uint32_t width, uint32_t height, uint16_t depthOrArraySize = 1 );

    
    virtual void CreateViews();

    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr) const override;

    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr) const override;

    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const;
     
    
    virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

	bool CheckSRVSupport()
	{
		return CheckFormatSupport(D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE);
	}

	bool CheckRTVSupport()
	{
		return CheckFormatSupport(D3D12_FORMAT_SUPPORT1_RENDER_TARGET);
	}

	bool CheckUAVSupport()
	{
		return CheckFormatSupport(D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW) &&
			CheckFormatSupport(D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD) &&
			CheckFormatSupport(D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE);
	}

	bool CheckDSVSupport()
	{
		return CheckFormatSupport(D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL);
	}

    static bool IsUAVCompatibleFormat(DXGI_FORMAT format);
    static bool IsSRGBFormat(DXGI_FORMAT format);
    static bool IsBGRFormat(DXGI_FORMAT format);
    static bool IsDepthFormat(DXGI_FORMAT format);

    
    static DXGI_FORMAT GetTypelessFormat(DXGI_FORMAT format);
	
	static DXGI_FORMAT GetSRGBFormat(DXGI_FORMAT format);
    static DXGI_FORMAT GetUAVCompatableFormat(DXGI_FORMAT format);

protected:

private:
    DescriptorAllocation CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc) const;
    DescriptorAllocation CreateUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc) const;

    mutable std::unordered_map<size_t, DescriptorAllocation> m_ShaderResourceViews;
    mutable std::unordered_map<size_t, DescriptorAllocation> m_UnorderedAccessViews;

    mutable std::mutex m_ShaderResourceViewsMutex;
    mutable std::mutex m_UnorderedAccessViewsMutex;

    DescriptorAllocation m_RenderTargetView;
    DescriptorAllocation m_DepthStencilView;

    TextureUsage m_TextureUsage;
};
#endif
