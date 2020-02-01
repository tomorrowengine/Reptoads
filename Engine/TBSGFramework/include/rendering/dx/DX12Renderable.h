#pragma once

#ifdef PLATFORM_WINDOWS
#include "rendering/RenderPass.h"
#include "rendering/Material.h"
#include "rendering/Mesh.h"
#include <gameplay/Transform.h>

namespace gfx
{
	struct DX12Renderable
	{
		DX12Renderable(core::Transform* a_transform, Mesh* a_mesh, Material* a_material,const ptl::string& id_) :
            transform(a_transform), mesh(a_mesh), material(a_material), id{id_} {}
		void SetMaterial(Material& a_material) { this->material = &a_material; }
		void SetTransform(core::Transform& a_transform) { this->transform = &a_transform; }
		core::Transform& GetTransform() { return *transform; }
		void SetMesh(Mesh& a_mesh) { this->mesh = &a_mesh; }
        ptl::string& GetId() { return id; };
		core::Transform* transform;
		Mesh* mesh;
		Material* material;
        ptl::string id;
	};
}
#endif
