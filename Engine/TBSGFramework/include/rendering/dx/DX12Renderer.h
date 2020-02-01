#pragma once
#include "DX12_Lights.h"
#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <rendering/Camera.h>
#include <rendering/IRenderer.h>

#include <rendering/dx/Window.h>
#include <rendering/dx/DirectXShapeMesh.h>
#include <rendering/dx/RenderTarget.h>
#include <rendering/dx/RootSignature.h>
#include <d3dx12.h>


namespace tbsg
{
    struct Config;
}

namespace gfx
{
	class DX12Renderer final : public IRenderer
	{
		friend class DxForwardPass;
		friend class DxMipmapPass;

		
		std::shared_ptr<Window> window_;
		HINSTANCE hInstance_;

		RenderTarget renderTarget_;
		RootSignature rootSignature_;

		
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

		D3D12_VIEWPORT viewport_;
		D3D12_RECT scissorRect_;

		bool justResizedDuringFrame = false;
		int windowClientWidth_;
		int windowClientHeight_;
		bool useVSync_;

	public:
		Texture m_EarthTexture;

	private:

		

		ptl::flat_value_map<gfx::PunctualRenderable, RenderableId> renderables;
		ptl::flat_value_map<ptl::unique_ptr<IRenderPass>, RenderPassId> renderPasses;


	private:
		DirectX::XMMATRIX perspectiveMatrix_;
		DirectX::XMMATRIX viewMatrix_;

		float zNear_ = 0.1f;
		float zFar_ = 100.0f;
		float aspect = 1920.0f / 1080.0f;
		float fov_ = 60;

		float left = 0.0f, right = 0.0f, forward = 0.0f, backward = 0.0f, up = 0.0f, down = 0.0f;
        float pitch = 0.0f, yaw = 0.0f;

		Camera* lastUsedCamera = nullptr;

	public:
		DX12Renderer(HINSTANCE hInstance, bool useVSync);
		
		void Initialize() override;
		
		void RenderWorld(Camera& camera) override;
		
		void RenderUI(Camera& camera) override;
		
		void Present() override;
		
		void Shutdown() override;


		D3D12_VIEWPORT GetViewport() const { return viewport_; }
        glm::vec<2, size_t> GetScreenSize() const override { return { viewport_.Width,viewport_.Height }; }
		D3D12_RECT GetScissorsRect() const { return scissorRect_; }
		RenderTarget& GetRenderTarget() { return renderTarget_; }

		RenderableId AddRenderable(core::Transform* transform, MeshId meshId, MaterialId materialId) override;
		bool ContainsRenderable(RenderableId renderableId) override;
		void RemoveRenderable(RenderableId renderableId) override;
		RenderPassId AddRenderPass(IRenderPass* renderPass) override;
		bool ContainsRenderPass(RenderPassId renderPassId) override;
		void RemoveRenderPass(RenderPassId renderPassId) override;

	private:
		void RenderRenderables(Camera& camera);
	};
}
#endif
