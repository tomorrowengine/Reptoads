#ifdef PLATFORM_WINDOWS
#include <core/Defines.h>
#include <rendering/dx/DX12LibPCH.h>
#include <rendering/dx/Buffer.h>

Buffer::Buffer(const std::wstring& name)
    : Resource(name)
{}

Buffer::Buffer(const D3D12_RESOURCE_DESC& resDesc,
	size_t numElements, size_t elementSize,
	const std::wstring& name)
	: Resource(resDesc, nullptr, name)
{
	UNUSED(numElements);
	UNUSED(elementSize);
}
#endif
