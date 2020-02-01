#pragma once
#include <cstddef>
#include <cstdint>
#include "memory/monotonic_resource.h"
#include "details/mem_options.h"



class RapidJsonAllocator
{
	struct Header
	{
		uint32_t size{};
	};
public:
    ~RapidJsonAllocator();
	

	
	static const bool kNeedFree = true;

	
	
	
	void* Malloc(std::size_t size);

	
	
	
	
	void* Realloc(void* originalPtr, size_t originalSize, size_t newSize);

	
	
	static void Free(void *ptr);

private:
	static void* InternalMallocImpl(std::size_t size, void* hint = nullptr);
	static Header& GetHeader(void* ptr);
};
