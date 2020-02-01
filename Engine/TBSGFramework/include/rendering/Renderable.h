#pragma once
#include "dx/DX12Renderable.h"
#include "ResourceIds.h"

namespace gfx
{
	struct Renderable
	{
		
		
		Renderable(MeshId meshId, MaterialId materialId) :
			meshId(meshId), materialId(materialId)
		{}

		MeshId meshId;
		MaterialId materialId;
		RenderableId renderableRegisteredId;
	};

	struct PunctualRenderable
	{
		core::Transform* transform;
		MeshId meshId;
		MaterialId materialId;
		RenderableId registeredId;
	};
}
