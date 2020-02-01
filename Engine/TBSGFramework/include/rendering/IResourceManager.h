#pragma once
#include <cstdint>
#include <iostream>

#include "Mesh.h"
#pragma warning(push)
#pragma warning( disable : 4127)
#include <glm/vec3.hpp>
#pragma warning( pop ) 
#include "Texture.h"
#include "Material.h"
#include "Shader.h"
#include "memory/smart_ptr.h"
#include "ResourceIds.h"

namespace gfx
{
	class IResourceManager
	{
	protected:
		using TextureMap = ptl::unordered_map<TextureId, gfx::Texture>;
		using MeshMap = ptl::unordered_map<MeshId, ptl::unique_ptr<gfx::Mesh>>;
		using MaterialMap = ptl::unordered_map<MaterialId, gfx::Material>;
		using ShaderMap = ptl::unordered_map<ShaderId, ptl::unique_ptr<IShader>>;
		using TextureNameMap = ptl::unordered_multimap<ptl::string, TextureId>;

		TextureMap textureMap{};
		TextureNameMap textureNameMap{};
		MeshMap meshMap{};
		MaterialMap materialMap{};
		ShaderMap shaderMap{};

	public:
		IResourceManager() = default;
		virtual ~IResourceManager() = default;
		TextureId GenerateTextureId() { ++m_textureId;  return TextureId{ m_textureId }; }
		ShaderId GenerateShaderId() { ++m_shaderId;  return ShaderId{ m_shaderId }; }
		MeshId GenerateMeshId() { ++m_meshId;  return MeshId{ m_meshId }; }
		MaterialId GenerateMaterialId() { ++m_materialId;  return MaterialId{ m_materialId }; }
		IResourceManager(const IResourceManager& other) = delete;
		IResourceManager(IResourceManager&& other) noexcept = delete;
		IResourceManager& operator=(const IResourceManager& other) = delete;
		IResourceManager& operator=(IResourceManager&& other) noexcept = delete;

		
		
		virtual TextureId AddTexture(gfx::Texture* inTexture) = 0;
		virtual TextureId CreateTexture(const ptl::string& filePath) = 0;
		
		virtual TextureId CreateTextureFromMemory(unsigned char* texBuffer, uint32_t width, uint32_t height, 
			uint32_t bitsPerChannel, uint32_t amountOfChannels, const glm::vec2 minUvs = { 0.0f, 0.0f }, const ptl::string& optionalName = "") = 0;
		virtual MeshId CreateMesh(const Vertex* vertices, size_t vtxCount, const uint32_t* indices, size_t idxCount, 
			const ptl::string& optionalName = "") = 0;
		virtual MaterialId CreateMaterial(BlendMode blendMode, glm::vec4 diffuseFactor, TextureId diffuseTextureId, 
			glm::vec3 emissiveFactor, TextureId emissiveTextureId, float alphaCutoff = 0.5f, const ptl::string& optionalName = "") = 0;
		
		virtual ShaderId CreateShader(const ptl::string& shaderPath) = 0;

		virtual Texture& GetDefaultTexture() = 0;
		virtual TextureId GetDefaultTextureId() = 0;
		virtual Texture& GetTexture(TextureId textureId);
		
		
		
		virtual Texture& GetTexture(const ptl::string& texturePath);
		virtual TextureId GetTextureId(const ptl::string& texturePath);
		virtual Mesh& GetMesh(MeshId meshId);
		virtual Material& GetMaterial(MaterialId materialId);
		virtual IShader& GetShader(ShaderId shaderId);
		
		virtual bool ContainsTexture(TextureId textureId);
		
		
		virtual bool ContainsTexture(const ptl::string& texturePath);
		virtual bool ContainsMesh(MeshId meshId);
		virtual bool ContainsMaterial(MaterialId materialId);
		virtual bool ContainsShader(ShaderId shaderId);

		virtual void DeleteTexture(TextureId textureId) = 0;
		virtual void DeleteMesh(MeshId meshId) = 0;
		virtual void DeleteMaterial(MaterialId materialId) = 0;
		virtual void DeleteShader(ShaderId shaderId) = 0;
		virtual void DeleteAllResources() = 0;
		void PrintCurrentMemoryUsage();
		uint32_t m_textureId{};
		uint32_t m_shaderId{};
		uint32_t m_meshId{};
		uint32_t m_materialId{};
	};
	inline Texture& IResourceManager::GetTexture(TextureId textureId)
	{
		if (textureId == GetDefaultTextureId())
			return GetDefaultTexture();

		auto tex_it = textureMap.find(textureId);
		if (tex_it == textureMap.end()) {
			ASSERT(false);
			return GetDefaultTexture();
		}

		return tex_it->second;
	}

	inline Texture& IResourceManager::GetTexture(const ptl::string& texturePath)
	{
		auto id_it = textureNameMap.find(texturePath);
		if(id_it == textureNameMap.end()) {
			ASSERT(false);
			return GetDefaultTexture();
		}

		auto& id = id_it->second;
		return GetTexture(id);
	}

	inline TextureId IResourceManager::GetTextureId(const ptl::string& texturePath)
	{
		auto id_it = textureNameMap.find(texturePath);
		if (id_it != textureNameMap.end()) {
			return id_it->second;
		}

		return TextureId{0};
	}

	inline Mesh& IResourceManager::GetMesh(MeshId meshId)
	{
		auto it = meshMap.find(meshId);
		if (it == meshMap.end()) {
			ASSERT(false);
		}

		return *it->second;
	}

	inline Material& IResourceManager::GetMaterial(MaterialId materialId)
	{
		auto it = materialMap.find(materialId);
		if (it == materialMap.end()) {
			ASSERT(false);
		}

		return it->second;
	}

	inline IShader& IResourceManager::GetShader(ShaderId shaderId)
	{
		auto it = shaderMap.find(shaderId);
		if (it == shaderMap.end()) {
			ASSERT(false);
		}

		return *it->second;
	}

	inline bool IResourceManager::ContainsTexture(TextureId textureId)
	{
		return textureMap.find(textureId) != textureMap.end();
	}

	inline bool IResourceManager::ContainsTexture(const ptl::string& texturePath)
	{
		auto id_it = textureNameMap.find(texturePath);
		if (id_it == textureNameMap.end()) {
			return false;
		}

		auto id = id_it->second;
		return ContainsTexture(id);
	}

	inline bool IResourceManager::ContainsMesh(MeshId meshId)
	{
		return meshMap.find(meshId) != meshMap.end();
	}

	inline bool IResourceManager::ContainsMaterial(MaterialId materialId)
	{
		return materialMap.find(materialId) != materialMap.end();
	}

	inline bool IResourceManager::ContainsShader(ShaderId shaderId)
	{
		return shaderMap.find(shaderId) != shaderMap.end();

	}

	inline void IResourceManager::PrintCurrentMemoryUsage()
	{
		std::cout << "Resource manager stats: \n"
			<< "Textures: " << textureMap.size() <<
			"\nMaterials: " << materialMap.size() <<
			"\nMeshes: " << meshMap.size() <<
			"\nShaders: " << shaderMap.size() << "\n";
	}
}
