#pragma once

#ifdef PLATFORM_WINDOWS
#include "rendering/dx/RootSignature.h"
#include "rendering/dx/Texture.h"
#include "rendering/dx/CommandList.h"
#include "rendering/dx/DX12Mesh.h"
#include "rendering/RenderPass.h"
#include "rendering/ResourceIds.h"

namespace gfx
{
	class Camera;
	class DX12Renderer;

	class DxSkyboxPass : public IRenderPass
	{
		Microsoft::WRL::ComPtr<ID3D12PipelineState> skyboxPipelineState;
		RootSignature skyboxRootSignature;
		Texture skyboxCubemap;
		DX12Mesh* skyboxMesh;

		DX12Renderer* renderer;

	public:
		
		RenderPassId renderPassId;

		DxSkyboxPass(DX12Renderer* renderer, ptl::string skyboxFile);

		~DxSkyboxPass() = default;
		DxSkyboxPass(const DxSkyboxPass& other) = delete;
		DxSkyboxPass(DxSkyboxPass&& other) noexcept = delete;
		DxSkyboxPass& operator=(const DxSkyboxPass& other) = delete;
		DxSkyboxPass& operator=(DxSkyboxPass&& other) = delete;
	
		void ExecutePass(Camera& camera, void* perFrameResources) override;
	};
}
#endif