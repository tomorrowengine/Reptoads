#include "rendering/unified_shader_passes/ParticlePass.h"

namespace gfx
{
#ifdef PLATFORM_WINDOWS
	void ParticlePass::ExecutePass(Camera& camera, void* perFrameResources)
	{
		UNUSED(perFrameResources);
		particleRenderer->RenderParticles(camera);
	}
#endif
}
