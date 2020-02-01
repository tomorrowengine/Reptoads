#pragma once




#ifdef PLATFORM_WINDOWS
#include "TextureUsage.h"

#include <d3d12.h>
#include <wrl.h>

#include <map> 
#include <memory> 
#include <mutex> 
#include <vector> 

class Buffer;
class ByteAddressBuffer;
class ConstantBuffer;
class DynamicDescriptorHeap;
class GenerateMipsPSO;
class IndexBuffer;
class PanoToCubemapPSO;
class RenderTarget;
class Resource;
class ResourceStateTracker;
class StructuredBuffer;
class RootSignature;
class Texture;
class UploadBuffer;
class VertexBuffer;

class CommandList
{
public:
	CommandList(D3D12_COMMAND_LIST_TYPE type);
	virtual ~CommandList();

	
	D3D12_COMMAND_LIST_TYPE GetCommandListType() const
	{
		return m_d3d12CommandListType;
	}

	
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> GetGraphicsCommandList() const
	{
		return m_d3d12CommandList;
	}

	
	void TransitionBarrier(const Resource& resource, D3D12_RESOURCE_STATES stateAfter, UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, bool flushBarriers = false);
	void TransitionBarrier(Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES stateAfter, UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, bool flushBarriers = false);

	
	void UAVBarrier(const Resource& resource, bool flushBarriers = false);
	void UAVBarrier(Microsoft::WRL::ComPtr<ID3D12Resource> resource, bool flushBarriers = false);

	
	void AliasingBarrier(const Resource& beforeResource, const Resource& afterResource, bool flushBarriers = false);
	void AliasingBarrier(Microsoft::WRL::ComPtr<ID3D12Resource> beforeResource, Microsoft::WRL::ComPtr<ID3D12Resource> afterResource, bool flushBarriers = false);

	
	void FlushResourceBarriers();

	
	void CopyResource(Resource& dstRes, const Resource& srcRes);
	void CopyResource(Microsoft::WRL::ComPtr<ID3D12Resource> dstRes, Microsoft::WRL::ComPtr<ID3D12Resource> srcRes);

	
	void ResolveSubresource(Resource& dstRes, const Resource& srcRes, uint32_t dstSubresource = 0, uint32_t srcSubresource = 0);

	
	void CopyVertexBuffer(VertexBuffer& vertexBuffer, size_t numVertices, size_t vertexStride, const void* vertexBufferData);
	template<typename T>
	void CopyVertexBuffer(VertexBuffer& vertexBuffer, const std::vector<T>& vertexBufferData)
	{
		CopyVertexBuffer(vertexBuffer, vertexBufferData.size(), sizeof(T), vertexBufferData.data());
	}

	
	void CopyIndexBuffer(IndexBuffer& indexBuffer, size_t numIndicies, DXGI_FORMAT indexFormat, const void* indexBufferData);
	template<typename T>
	void CopyIndexBuffer(IndexBuffer& indexBuffer, const std::vector<T>& indexBufferData)
	{
		assert(sizeof(T) == 2 || sizeof(T) == 4);

		DXGI_FORMAT indexFormat = (sizeof(T) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		CopyIndexBuffer(indexBuffer, indexBufferData.size(), indexFormat, indexBufferData.data());
	}

	
	void CopyByteAddressBuffer(ByteAddressBuffer& byteAddressBuffer, size_t bufferSize, const void* bufferData);
	template<typename T>
	void CopyByteAddressBuffer(ByteAddressBuffer& byteAddressBuffer, const T& data)
	{
		CopyByteAddressBuffer(byteAddressBuffer, sizeof(T), &data);
	}

	
	void CopyStructuredBuffer(StructuredBuffer& structuredBuffer, size_t numElements, size_t elementSize, const void* bufferData);
	template<typename T>
	void CopyStructuredBuffer(StructuredBuffer& structuredBuffer, const std::vector<T>& bufferData)
	{
		CopyStructuredBuffer(structuredBuffer, bufferData.size(), sizeof(T), bufferData.data());
	}

	
	void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitiveTopology);

	
	void LoadTextureFromFile(Texture& texture, const std::wstring& fileName, TextureUsage textureUsage = TextureUsage::Albedo);

	
	void ClearTexture(const Texture& texture, const float clearColor[4]);

	
	void ClearDepthStencilTexture(const Texture& texture, D3D12_CLEAR_FLAGS clearFlags, float depth = 1.0f, uint8_t stencil = 0);

	
	void GenerateMips(Texture& texture);

	
	void PanoToCubemap(Texture& cubemap, const Texture& pano);

	
	void CopyTextureSubresource(Texture& texture, uint32_t firstSubresource, uint32_t numSubresources, D3D12_SUBRESOURCE_DATA* subresourceData);

	
	void SetGraphicsDynamicConstantBuffer(uint32_t rootParameterIndex, size_t sizeInBytes, const void* bufferData);
	template<typename T>
	void SetGraphicsDynamicConstantBuffer(uint32_t rootParameterIndex, const T& data)
	{
		SetGraphicsDynamicConstantBuffer(rootParameterIndex, sizeof(T), &data);
	}

	
	void SetComputeDynamicConstantBuffer(uint32_t rootParameterIndex, size_t sizeInBytes, const void* bufferData);
	template<typename T>
	void SetComputeDynamicConstantBuffer(uint32_t rootParameterIndex, const T& data)
	{
		SetComputeDynamicConstantBuffer(rootParameterIndex, sizeof(T), &data);
	}

	
	void SetGraphics32BitConstants(uint32_t rootParameterIndex, uint32_t numConstants, const void* constants);
	template<typename T>
	void SetGraphics32BitConstants(uint32_t rootParameterIndex, const T& constants)
	{
		static_assert(sizeof(T) % sizeof(uint32_t) == 0, "Size of type must be a multiple of 4 bytes");
		SetGraphics32BitConstants(rootParameterIndex, sizeof(T) / sizeof(uint32_t), &constants);
	}

	
	void SetCompute32BitConstants(uint32_t rootParameterIndex, uint32_t numConstants, const void* constants);
	template<typename T>
	void SetCompute32BitConstants(uint32_t rootParameterIndex, const T& constants)
	{
		static_assert(sizeof(T) % sizeof(uint32_t) == 0, "Size of type must be a multiple of 4 bytes");
		SetCompute32BitConstants(rootParameterIndex, sizeof(T) / sizeof(uint32_t), &constants);
	}


	
	void SetVertexBuffer(uint32_t slot, const VertexBuffer& vertexBuffer);

	
	void SetDynamicVertexBuffer(uint32_t slot, size_t numVertices, size_t vertexSize, const void* vertexBufferData);
	template<typename T>
	void SetDynamicVertexBuffer(uint32_t slot, const std::vector<T>& vertexBufferData)
	{
		SetDynamicVertexBuffer(slot, vertexBufferData.size(), sizeof(T), vertexBufferData.data());
	}

	
	void SetIndexBuffer(const IndexBuffer& indexBuffer);

	
	void SetDynamicIndexBuffer(size_t numIndicies, DXGI_FORMAT indexFormat, const void* indexBufferData);
	template<typename T>
	void SetDynamicIndexBuffer(const std::vector<T>& indexBufferData)
	{
		static_assert(sizeof(T) == 2 || sizeof(T) == 4, "Only uint32_t and uint16_t sizes are supported");

		DXGI_FORMAT indexFormat = (sizeof(T) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		SetDynamicIndexBuffer(indexBufferData.size(), indexFormat, indexBufferData.data());
	}

	
	void SetGraphicsDynamicStructuredBuffer(uint32_t slot, size_t numElements, size_t elementSize, const void* bufferData);
	template<typename T>
	void SetGraphicsDynamicStructuredBuffer(uint32_t slot, const std::vector<T>& bufferData)
	{
		SetGraphicsDynamicStructuredBuffer(slot, bufferData.size(), sizeof(T), bufferData.data());
	}

	
	void SetViewport(const D3D12_VIEWPORT& viewport);
	void SetViewports(const std::vector<D3D12_VIEWPORT>& viewports);

	
	void SetScissorRect(const D3D12_RECT& scissorRect);
	void SetScissorRects(const std::vector<D3D12_RECT>& scissorRects);

	
	void SetPipelineState(Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState);

	
	void SetGraphicsRootSignature(const RootSignature& rootSignature);
	void SetComputeRootSignature(const RootSignature& rootSignature);

	
	void SetShaderResourceView(
		uint32_t rootParameterIndex,
		uint32_t descriptorOffset,
		const Resource& resource,
		D3D12_RESOURCE_STATES stateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE |
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
		UINT firstSubresource = 0,
		UINT numSubresources = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
		const D3D12_SHADER_RESOURCE_VIEW_DESC* srv = nullptr
	);

	
	void SetUnorderedAccessView(
		uint32_t rootParameterIndex,
		uint32_t descrptorOffset,
		const Resource& resource,
		D3D12_RESOURCE_STATES stateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		UINT firstSubresource = 0,
		UINT numSubresources = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
		const D3D12_UNORDERED_ACCESS_VIEW_DESC* uav = nullptr
	);

	
	void SetRenderTarget(const RenderTarget& renderTarget);

	
	void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t startVertex = 0, uint32_t startInstance = 0);
	void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t startIndex = 0, int32_t baseVertex = 0, uint32_t startInstance = 0);

	
	void Dispatch(uint32_t numGroupsX, uint32_t numGroupsY = 1, uint32_t numGroupsZ = 1);

	

	 
	bool Close(CommandList& pendingCommandList);
	
	void Close();

	
	void Reset();

	
	void ReleaseTrackedObjects();

	
	void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, ID3D12DescriptorHeap* heap);

	std::shared_ptr<CommandList> GetGenerateMipsCommandList() const
	{
		return m_ComputeCommandList;
	}

protected:

private:
	void TrackResource(Microsoft::WRL::ComPtr<ID3D12Object> object);
	void TrackResource(const Resource& res);

	
	void GenerateMips_UAV(Texture& texture, bool isSRGB);

	
	void CopyBuffer(Buffer& buffer, size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

	
	void BindDescriptorHeaps();

	using TrackedObjects = std::vector < Microsoft::WRL::ComPtr<ID3D12Object> >;

	D3D12_COMMAND_LIST_TYPE m_d3d12CommandListType;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> m_d3d12CommandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_d3d12CommandAllocator;

	
	
	
	
	std::shared_ptr<CommandList> m_ComputeCommandList;

	
	
	ID3D12RootSignature* m_RootSignature;

	
	
	std::unique_ptr<UploadBuffer> m_UploadBuffer;

	
	
	
	std::unique_ptr<ResourceStateTracker> m_ResourceStateTracker;

	
	
	
	std::unique_ptr<DynamicDescriptorHeap> m_DynamicDescriptorHeap[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

	
	
	ID3D12DescriptorHeap* m_DescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

	
	std::unique_ptr<GenerateMipsPSO> m_GenerateMipsPSO;
	
	std::unique_ptr<PanoToCubemapPSO> m_PanoToCubemapPSO;

	
	
	
	
	
	TrackedObjects m_TrackedObjects;

	
	static std::map<std::wstring, ID3D12Resource* > ms_TextureCache;
	static std::mutex ms_TextureCacheMutex;
};
#endif
