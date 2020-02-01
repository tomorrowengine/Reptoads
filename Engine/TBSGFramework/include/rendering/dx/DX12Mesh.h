#pragma once
#ifdef PLATFORM_WINDOWS
#include <rendering/Mesh.h>
#include <rendering/dx/IndexBuffer.h>
#include <rendering/dx/VertexBuffer.h>

namespace gfx
{
	class DX12Mesh final : public Mesh
	{
		VertexBuffer vertexBuffer_;
		IndexBuffer indexBuffer_;

		std::string name_;

	public:
		
		DX12Mesh(const Vertex* a_vertices, size_t a_vtxCount, const uint32_t* a_indices,
			size_t a_idxCount, const char* a_id);

		void UploadBuffers(const Vertex* a_vertices, size_t a_vtxCount, const uint32_t* a_indices,
			size_t a_idxCount) override;

		VertexBuffer* GetVertexBuffer()
		{
			return &vertexBuffer_;
		}

		IndexBuffer* GetIndexBuffer()
		{
			return &indexBuffer_;
		}
	};
}

#endif
