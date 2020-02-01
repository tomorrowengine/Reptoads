#pragma once
#ifdef PLATFORM_WINDOWS
#include <rendering/IResourceManager.h>
#include <rendering/Texture.h>
#include <rendering/Mesh.h>
#include <rendering/Material.h>
#include <rendering/RenderPass.h>
#include <rendering/dx/DX12Mesh.h>

namespace gfx
{
	class DX12Renderer;

	class DX12ResourceManager : public IResourceManager
	{
		DX12Renderer* renderer_;
		
		static uint32_t textureIdCounter;
		static uint32_t meshIdCounter;
		static uint32_t materialIdCounter;
		static uint32_t shaderIdCounter;

		const TextureId defaultTextureId{ std::numeric_limits<uint32_t>::max() - 1 };

	public:
		DX12ResourceManager(DX12Renderer* renderer);

		TextureId CreateTexture(const ptl::string& path) override;
		TextureId AddTexture(gfx::Texture* inTexture) override;
		TextureId CreateTextureFromMemory(unsigned char* texBuffer, uint32_t width, uint32_t height, 
			uint32_t bitsPerChannel, uint32_t amountOfChannels, const glm::vec2 minUvs, const ptl::string& optionalName = "") override;
		MeshId CreateMesh(const Vertex* vertices, size_t vtxCount, const uint32_t* indices, size_t idxCount, 
			const ptl::string& optionalName = "") override;
		MaterialId CreateMaterial(BlendMode blendMode, glm::vec4 diffuseFactor, TextureId diffuseTextureId, 
			glm::vec3 emissiveFactor, TextureId emmisiveTextureId, float alphaCutoff = 0.5f, const ptl::string& optionalName = "") override;
		ShaderId CreateShader(const ptl::string& shaderPath) override;


		Texture& GetDefaultTexture() override;
		TextureId GetDefaultTextureId() override;

		
		
		
		
		

		
		
		
		
		

		void DeleteTexture(TextureId textureId) override;
		void DeleteMesh(MeshId meshId) override;
		void DeleteMaterial(MaterialId materialId) override;
		void DeleteShader(ShaderId shaderId) override;
		void DeleteAllResources() override;
	};

}
#endif
