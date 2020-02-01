#include <rendering/dx/DX12LibPCH.h>
#ifdef PLATFORM_WINDOWS
#pragma warning( disable : 4238)
#include <rendering/dx/UploadBuffer.h>

#include <rendering/dx/DX12Application.h>
#include <rendering/dx/Helpers.h>

UploadBuffer::UploadBuffer(size_t pageSize)
	: m_PageSize(pageSize)
{}

UploadBuffer::~UploadBuffer()
{}

UploadBuffer::Allocation UploadBuffer::Allocate(size_t sizeInBytes, size_t alignment)
{
	
	
	if (!m_CurrentPage || !m_CurrentPage->HasSpace(sizeInBytes, alignment)) {
		while (sizeInBytes > m_PageSize) {
			m_PageSize *= 2;
		}
		m_CurrentPage = RequestPage(sizeInBytes, alignment);
	}

	return m_CurrentPage->Allocate(sizeInBytes, alignment);
}

std::shared_ptr<UploadBuffer::Page> UploadBuffer::RequestPage(size_t sizeInBytes, size_t alignment)
{
	std::shared_ptr<Page> page;


	auto iter = m_AvailablePages.begin();
	while (iter != m_AvailablePages.end()) {
		auto _page = *iter;
		if (_page->HasSpace(sizeInBytes, alignment)) {
			page = _page;
			iter = m_AvailablePages.erase(iter);
		} else {
			++iter;
		}
	}
	if (!page) {
		page = std::make_shared<Page>(m_PageSize);
		m_PagePool.push_back(page);
	}

	return page;
}

void UploadBuffer::Reset()
{
	m_CurrentPage = nullptr;
	
	m_AvailablePages = m_PagePool;

	for (auto page : m_AvailablePages) {
		
		page->Reset();
	}
}

UploadBuffer::Page::Page(size_t sizeInBytes)
	: m_PageSize(sizeInBytes)
	, m_Offset(0)
	, m_CPUPtr(nullptr)
	, m_GPUPtr(D3D12_GPU_VIRTUAL_ADDRESS(0))
{
	auto device = DX12Application::Get().GetDevice();

	ThrowIfFailed(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_PageSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_d3d12Resource)
	));

	m_GPUPtr = m_d3d12Resource->GetGPUVirtualAddress();
	m_d3d12Resource->Map(0, nullptr, &m_CPUPtr);
}

UploadBuffer::Page::~Page()
{
	m_d3d12Resource->Unmap(0, nullptr);
	m_CPUPtr = nullptr;
	m_GPUPtr = D3D12_GPU_VIRTUAL_ADDRESS(0);
}

bool UploadBuffer::Page::HasSpace(size_t sizeInBytes, size_t alignment) const
{
	size_t alignedSize = Math::AlignUp(sizeInBytes, alignment);
	size_t alignedOffset = Math::AlignUp(m_Offset, alignment);

	return alignedOffset + alignedSize <= m_PageSize;
}

UploadBuffer::Allocation UploadBuffer::Page::Allocate(size_t sizeInBytes, size_t alignment)
{
	if (!HasSpace(sizeInBytes, alignment)) {
		
		throw std::bad_alloc();
	}

	size_t alignedSize = Math::AlignUp(sizeInBytes, alignment);
	m_Offset = Math::AlignUp(m_Offset, alignment);

	Allocation allocation;
	allocation.CPU = static_cast<uint8_t*>(m_CPUPtr) + m_Offset;
	allocation.GPU = m_GPUPtr + m_Offset;

	m_Offset += alignedSize;

	return allocation;
}

void UploadBuffer::Page::Reset()
{
	m_Offset = 0;
}
#endif
