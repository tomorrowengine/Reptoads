#pragma once
#include "rendering/Camera.h"


namespace gfx
{

	class IRenderPass
	{
	public:
		IRenderPass() = default;
		virtual ~IRenderPass() = default;

		IRenderPass(const IRenderPass& other) = delete;
		IRenderPass(IRenderPass&& other) noexcept = delete;
		IRenderPass& operator=(const IRenderPass& other) = delete;
		IRenderPass& operator=(IRenderPass&& other) noexcept = delete;

		virtual void ExecutePass(Camera& camera, void* perFrameResources) = 0;
	};
}
