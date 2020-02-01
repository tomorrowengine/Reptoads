#pragma once
#ifdef PLATFORM_WINDOWS
#include "rendering/RenderPass.h"
#include "rendering/Texture.h"


namespace gfx
{
	class DX12Renderer;

	class DxMipmapPass : public IRenderPass
	{
	public:
		DxMipmapPass(DX12Renderer* renderer, Texture* texture);

		void ExecutePass(Camera& camera, void* perFrameResources) override;

		Texture& GetNonMsaaTextureToMipmap() { return nonMsaaTextureToMipmap; }

	private:
		void CreateNonMSAATexture();

	private:
		DX12Renderer* renderer;
		Texture* textureToMipmap;
		Texture nonMsaaTextureToMipmap;
		bool isNonMsaaTextureToMipmapCreated = false;
	};
}
#endif PLATFORM_WINDOWS
