#pragma once
#include "memory/Containers.h"
#include "rendering/dx/DX12Renderer.h"
#ifdef PLATFORM_WINDOWS
#include <cstdint>
#include <unordered_map>
#include "rendering/Shader.h"
#include "rendering/dx/RootSignature.h"
#include "rendering/RenderPass.h"


class CommandList;


namespace gfx
{
	class DX12Renderer;

	class DxForwardPass : public IRenderPass
	{
	public:
		RenderPassId renderPassId;
		
		DxForwardPass(DX12Renderer* renderer, ptl::vector<IShader*> shaders);
		Texture& GetBloomTexture(){ return rwBloomTexture; }
		
		~DxForwardPass() = default;
		DxForwardPass(const DxForwardPass& other) = delete;
		DxForwardPass(DxForwardPass&& other) noexcept = delete;
		DxForwardPass& operator=(const DxForwardPass& other) = delete;
		DxForwardPass& operator=(DxForwardPass&& other) = delete;


		void ExecutePass(Camera& camera, void* perFrameResources) override;

		Texture& GetRwBloomTexture() { return rwBloomTexture; }
		DXGI_FORMAT GetBloomTextureFormat() const { return bloomTextureFormat; }

	private:
		void PrepareForRendering(CommandList& commandList);

	public:
		const DXGI_FORMAT bloomTextureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	private:
		DX12Renderer* renderer;
		std::unordered_map<ShaderType, IShader*> shaders_;

		RootSignature rootSignature_;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

		Texture rwBloomTexture;
	};
}
#endif
