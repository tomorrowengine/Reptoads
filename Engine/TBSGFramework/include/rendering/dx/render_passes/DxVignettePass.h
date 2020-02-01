#pragma once
#ifdef PLATFORM_WINDOWS
#include "rendering/RenderPass.h"
#include "rendering/dx/DX12Renderer.h"

namespace gfx
{
	class DxVignettePass : public IRenderPass
	{
	public:
		DxVignettePass(DX12Renderer* renderer, float vignetteRadiusZeroToOne, float vignetteSmoothnessZeroToOne);


		void ExecutePass(Camera& camera, void* perFrameResources) override;

		float vignetteRadiusZeroToOne;
		float vignetteSmoothnessZeroToOne;

	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> vignettePipelineState;
		RootSignature vignetteRootSignature;
		VertexBuffer vertexBuffer{ L"Vignette Fullscreen vertex buffer" };
		IndexBuffer indexBuffer{ L"Vignette Fullscreen index buffer" };

		DX12Renderer* renderer;
	};
}
#endif