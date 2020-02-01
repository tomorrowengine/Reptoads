#pragma once
#ifdef PLATFORM_WINDOWS
#include "rendering/RenderPass.h"
#include "rendering/Texture.h"

#include "rendering/dx/RootSignature.h"
#include "rendering/dx/VertexBuffer.h"
#include "rendering/dx/IndexBuffer.h"


namespace gfx
{
	class DxMipmapPass;
	class DX12Renderer;

	class DxBlurPass : public IRenderPass
	{
	public:
		DxBlurPass(DX12Renderer* renderer, DxMipmapPass& mipmapPass);

		void ExecutePass(Camera& camera, void* perFrameResources) override;

	private:
		DX12Renderer* renderer;
		DxMipmapPass* mipmapPass;
		Texture horizontalBlurredTexture;
		DXGI_FORMAT textureFormat;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> blurPipelineState;
		RootSignature blurRootSignature;
		VertexBuffer vertexBuffer{ L"Blur Fullscreen vertex buffer" };
		IndexBuffer indexBuffer{ L"Blur Fullscreen index buffer" };
	};
}
#endif
