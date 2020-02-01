#pragma once
#include "ResourceIds.h"
#include "memory/smart_ptr.h"
#include "shader_headers/DefaultShaderBuffers.h"
#include "memory/default_memresource.h"

namespace gfx
{
	enum class BlendMode {
		
		Opaque,
		
		Masked,
		
		Blending,
	};
	

	class Material
	{
	public:
		Material(BlendMode blendMode, float alphaCutoff, glm::vec4 diffuseFactor, TextureId diffuseTextureId, 
			glm::vec3 emissiveFactor, TextureId emissiveTextureId)
			: blendMode(blendMode), diffuseTextureId(diffuseTextureId), emissiveTextureId(emissiveTextureId), 
			heapShaderMaterial{new ShaderMaterial{}}
		{
			heapShaderMaterial->materialDiffuseColor = diffuseFactor;
			heapShaderMaterial->materialEmissiveColor = emissiveFactor;
			heapShaderMaterial->alphaCutoff = alphaCutoff;
		}

		const BlendMode blendMode;
		const TextureId diffuseTextureId;
		const TextureId emissiveTextureId;
		ptl::string name;


		
		
		ShaderMaterial& GetShaderMaterialData() { return *heapShaderMaterial; }

	private:
		ptl::unique_ptr<ShaderMaterial> heapShaderMaterial;
	};
}
