#include <rendering/dx/DX12Material.h>

namespace gfx
{
	DX12Material::DX12Material(const glm::vec3& , const glm::vec3& , const glm::vec3& ,
		float , const char* )
		: m_data(nullptr),
		m_hasTexture(false), m_texId{}
	{
	}

	DX12Material::DX12Material(const glm::vec3& , const glm::vec3& ,
		const glm::vec3& , float , const char* a_textureId, const char* )
		: m_data(nullptr),
		m_hasTexture(true), m_texId(a_textureId)
	{
	}
}
