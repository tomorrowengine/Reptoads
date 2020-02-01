#pragma once
#ifdef PLATFORM_WINDOWS
#include "rendering/IParticleRenderer.h"
#include "RootSignature.h"

namespace gfx
{
	class DX12Renderer;

	class DxParticleRenderer : public IParticleRenderer
	{
	public:
		explicit DxParticleRenderer(DX12Renderer* renderer, Texture& bloomTexture, DXGI_FORMAT bloomTextureFormat);

		void RenderParticles(Camera& camera);

	private:
		void RenderParticleEmitter(core::Transform& transform, ParticleEmitter& emitter, Camera& camera);

	private:
		DX12Renderer* renderer;
		const Texture* bloomTex;
		const DXGI_FORMAT bloomTexFormat;

		RootSignature particleRootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> backgroundParticlePipelineState;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> particlePipelineState;
	};
}

#endif