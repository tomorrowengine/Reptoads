#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace gfx
{
	enum VertexAttribute
	{
		vertexPosition	=	1 << 0,
		vertexColor		=	1 << 1,
		vertexNormal	=	1 << 2,
		vertexUv		=	1 << 3,
		ATTRIBUTE_END_BIT = 1 << 4
	};
	
	
	
	struct Vertex
	{
		glm::vec3 pos;	  
		glm::vec3 color;	  
		glm::vec3 normal; 
		glm::vec2 uv;		  
	};

	class Mesh
	{
	public:
		virtual ~Mesh() = default;
		Mesh() = default;
		Mesh(uint32_t vtxCount, uint32_t idxCount) : m_vtxCount(vtxCount), m_idxCount(idxCount) {}
		
		virtual void UploadBuffers(const Vertex* a_vertices, size_t a_vtxCount, const uint32_t* a_indices, size_t a_idxCount) = 0;
		uint32_t GetVtxCount() const { return m_vtxCount; }
		uint32_t GetIdxCount() const { return m_idxCount; }

	protected:
		uint32_t m_vtxCount = 0;
		uint32_t m_idxCount = 0;
	};

}
