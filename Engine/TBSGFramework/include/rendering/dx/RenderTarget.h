
#pragma once



#ifdef PLATFORM_WINDOWS

#include <cstdint>
#include <vector>

#include <DirectXMath.h> 

#include "Texture.h"

 

enum AttachmentPoint
{
    Color0,
    Color1,
    Color2,
    Color3,
    Color4,
    Color5,
    Color6,
    Color7,
    DepthStencil,
    NumAttachmentPoints
};

class RenderTarget
{
public:
    
    RenderTarget();

    RenderTarget( const RenderTarget& copy ) = default;
    RenderTarget( RenderTarget&& copy ) = default;

    RenderTarget& operator=( const RenderTarget& other ) = default;
    RenderTarget& operator=( RenderTarget&& other ) = default;

	~RenderTarget() = default;

    
    
    void AttachTexture( AttachmentPoint attachmentPoint, const Texture& texture );
    const Texture& GetTexture( AttachmentPoint attachmentPoint ) const;

    
	void Resize(DirectX::XMUINT2 size);
	void Resize(uint32_t width, uint32_t height);
	DirectX::XMUINT2 GetSize() const;
	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	
	
	
	
	D3D12_VIEWPORT GetViewport(DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, DirectX::XMFLOAT2 bias = { 0.0f, 0.0f }, float minDepth = 0.0f, float maxDepth = 1.0f) const;


    
    
    
    const std::vector<Texture>& GetTextures() const;

    
    
    
    D3D12_RT_FORMAT_ARRAY GetRenderTargetFormats() const;

    
    DXGI_FORMAT GetDepthStencilFormat() const;

private:
    
    std::vector<Texture> m_Textures;
	DirectX::XMUINT2 m_Size;
};
#endif
