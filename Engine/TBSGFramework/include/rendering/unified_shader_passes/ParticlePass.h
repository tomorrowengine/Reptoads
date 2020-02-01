#pragma once
#ifdef PLATFORM_WINDOWS
#include "rendering/RenderPass.h"
#include "rendering/dx/DxParticleRenderer.h"

namespace gfx
{
	class ParticlePass : public IRenderPass
	{
	public:
		ParticlePass(DxParticleRenderer* particleRenderer) :
		particleRenderer(particleRenderer){}
		void ExecutePass(Camera& camera, void* perFrameResources);

	private:
		DxParticleRenderer* particleRenderer;
	};
}
#endif
