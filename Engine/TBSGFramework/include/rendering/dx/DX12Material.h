#pragma once
#include <glm/glm.hpp>
#include "core/Assertion.h"
#include "memory/String.h"

struct MaterialDataPs;

namespace gfx
{
	class DX12Material
	{
	public:
		DX12Material(const glm::vec3& a_ambient, const glm::vec3& a_diffuse,
			const glm::vec3& a_specular, float a_specularPower, const char* a_id);

		DX12Material(const glm::vec3& a_ambient, const glm::vec3& a_diffuse,
			const glm::vec3& a_specular, float a_specularPower, const char* a_textureId, const char* a_id);

		virtual ~DX12Material() = default;
		DX12Material(const DX12Material& other) = delete;
		DX12Material(DX12Material&& other) noexcept = default;
		DX12Material& operator=(const DX12Material& other) = delete;
		DX12Material& operator=(DX12Material&& other) noexcept = default;
		void SetTexture(const char* a_texId) { ASSERT_MSG(m_hasTexture, "THIS MATERIAL DOESN'T USE TEXTURES..."); m_texId = a_texId; }
		bool HasTexture() const { return m_hasTexture; }
		const ptl::string& GetTexId() const { return m_texId; }
		const MaterialDataPs& GetMaterialData() const { return *m_data; }
	protected:
		
		MaterialDataPs* m_data{};
		bool m_hasTexture{};
		ptl::string m_texId{};
	};
}
