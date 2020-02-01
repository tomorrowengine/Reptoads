#ifdef PLATFORM_WINDOWS
#include "rendering/dx/DX12Application.h"
#include <rendering/dx/DX12Mesh.h>
#include <rendering/dx/CommandList.h>
#include <rendering/dx/CommandQueue.h>

#include <codecvt>


namespace gfx
{
	DX12Mesh::DX12Mesh(const Vertex* a_vertices, size_t a_vtxCount, const uint32_t* a_indices,
		size_t a_idxCount, const char* a_id)
	{
		name_ = a_id;
		m_vtxCount = static_cast<uint32_t>(a_vtxCount);
		m_idxCount = static_cast<uint32_t>(a_idxCount);

		DX12Mesh::UploadBuffers(a_vertices, a_vtxCount, a_indices, a_idxCount);
	}

	void DX12Mesh::UploadBuffers(const Vertex* a_vertices, size_t a_vtxCount, const uint32_t* a_indices,
		size_t a_idxCount)
	{
		auto commandQueue = DX12Application::Get().GetCommandQueue();
		auto commandList = commandQueue->GetCommandList();

		commandList->CopyVertexBuffer(vertexBuffer_, a_vtxCount, sizeof(Vertex), a_vertices);
		commandList->CopyIndexBuffer(indexBuffer_, a_idxCount, DXGI_FORMAT_R32_UINT, a_indices);

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		vertexBuffer_.SetName(converter.from_bytes(name_ + " (Vertex Buffer)"));
		indexBuffer_.SetName(converter.from_bytes(name_ + " (Index Buffer)"));

		auto fence = commandQueue->ExecuteCommandList(commandList);
		commandQueue->WaitForFenceValue(fence);
	}
}

#endif
