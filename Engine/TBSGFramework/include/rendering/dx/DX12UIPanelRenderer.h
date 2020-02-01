#pragma once
#ifdef PLATFORM_WINDOWS
#include "RootSignature.h"
#include "IndexBuffer.h"
#include <glm/glm.hpp>
#include <array>

#include <rendering/dx/CommandQueue.h>
#include <rendering/dx/CommandList.h>
#include "rendering/IUIRenderer.h"

namespace gfx
{
	class DX12Renderer;

	class DX12UIPanelRenderer
	{
	public:
		explicit DX12UIPanelRenderer(DX12Renderer* renderer);

		void Initialize();
		void StartFrame();
		void RenderPanel(gfx::IUIRenderer::CachedUIImage* uiPanel);
		void RenderPanel(gfx::IUIRenderer::CachedUIText* uiPanel);
		void EndFrame();
		void Shutdown();

		void AddImage(gfx::IUIRenderer::CachedUIImage& uiImage);
		void UpdateImage(gfx::IUIRenderer::CachedUIImage& uiImage);
		void RemoveImage(ui::UIImage& uiImage);

	private:
		DX12Renderer* renderer;

		
		RootSignature uiOverlayRootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> uiOverlayPipelineState;

		std::shared_ptr<CommandQueue> commandQueue_ {};
		std::shared_ptr<CommandList> commandList_ {};

		IndexBuffer indexBuffer;
		

		

	private:









	};
}
#endif
