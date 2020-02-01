
#ifdef PLATFORM_WINDOWS
#include "rendering/dx/render_passes/DxMipmapPass.h"
#include "core/Defines.h"
#include "rendering/dx/DX12Application.h"
#include "rendering/dx/CommandQueue.h"
#include "rendering/dx/CommandList.h"
#include "rendering/dx/DX12Renderer.h"


namespace gfx
{
	DxMipmapPass::DxMipmapPass(DX12Renderer* renderer, Texture* texture) : renderer(renderer), textureToMipmap(texture)
	{
		
	}

	void DxMipmapPass::ExecutePass(Camera& camera, void* perFrameResources)
	{
		UNUSED(perFrameResources);
		UNUSED(camera);
		

		if(renderer->justResizedDuringFrame) {
			nonMsaaTextureToMipmap.Resize(renderer->windowClientWidth_, renderer->windowClientHeight_);
			nonMsaaTextureToMipmap.CreateViews();
		}

		bool isFirstFrame = false;
		if (!isNonMsaaTextureToMipmapCreated) {
			CreateNonMSAATexture();
			isFirstFrame = true;
		}

		auto commandQueue = DX12Application::Get().GetCommandQueue();
		auto resolveCommandList = commandQueue->GetCommandList();
		auto bugFixCommandList = commandQueue->GetCommandList();
		auto mipCommandList = commandQueue->GetCommandList();

		resolveCommandList->ResolveSubresource(nonMsaaTextureToMipmap, *textureToMipmap);
		

		if (!isFirstFrame && !renderer->justResizedDuringFrame) {
			
			
		}

		mipCommandList->GenerateMips(nonMsaaTextureToMipmap);

		commandQueue->ExecuteCommandList(resolveCommandList);
		commandQueue->ExecuteCommandList(bugFixCommandList);
		auto fence = commandQueue->ExecuteCommandList(mipCommandList);
		commandQueue->WaitForFenceValue(fence);
	}

	void DxMipmapPass::CreateNonMSAATexture()
	{
		auto desc = textureToMipmap->GetD3D12ResourceDesc();
		desc.Alignment = 65536;
		desc.Flags &= ~(D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		desc.MipLevels = 8;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		nonMsaaTextureToMipmap = Texture(desc, nullptr, TextureUsage::Albedo, L"NonMsaa Texture to Mipmap");
		nonMsaaTextureToMipmap.CreateViews();


		isNonMsaaTextureToMipmapCreated = true;
	}
}
#endif
