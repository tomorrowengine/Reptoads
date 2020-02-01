#pragma once
#pragma warning(push, 0)   
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tinygltf/new_new_tiny_gltf.h>
#pragma warning(pop)
#include "memory/String.h"
#include "rendering/Mesh.h"
#include "rendering/IResourceManager.h"
#include "memory/Containers.h"
#include "gameplay/Transform.h"


namespace scene
{
    class SceneNode;
}


namespace gfx {
	
	struct RenderableMesh
	{
		MeshId meshId;
		gfx::Mesh* mesh;
		MaterialId materialId;
		gfx::Material* material;
	};
    class IRenderer;
}

class GLTFLoader
{
    gfx::IRenderer& renderer_;
	gfx::IResourceManager* gpuResourceManager_;
	ptl::string gltfFile_ = "";
	scene::SceneNode* rootSceneNode_ = nullptr;
	tinygltf::Model model_;
	ptl::unordered_map<int , MaterialId> materialMap_;
	ptl::unordered_map<int , ptl::vector<gfx::RenderableMesh> > renderableMeshMap_;

public:
	explicit GLTFLoader(gfx::IRenderer& gpuResourceManager);

	
	bool LoadModelFromGltf(const ptl::string& gltfFile, scene::SceneNode* rootSceneNode = nullptr);

private:
	bool LoadModelFromGltfOrGlb(const ptl::string &filename, tinygltf::Model *model);

	
	void AddNodeToScene(int nodeIndex, scene::SceneNode* parentSceneNode);

    
    void AddCameraToScene(tinygltf::Node& camera);
	
	
	void CreateMeshMaterialTexture(int meshIndex, scene::SceneNode* sceneNode);
    void CreateLights();
	
	MaterialId CreateMaterial(int materialIndex);
	ptl::string GetNameForMaterial(int materialIndex);
	ptl::string GetNameForTexture(int textureIndex);
	ptl::string GetNameForMesh(int meshIndex, int primitiveIndex);
	static core::Transform GetLocalTransformFromNode(tinygltf::Node& node, scene::SceneNode* parentNode);
};
