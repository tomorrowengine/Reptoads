#pragma once
#include <cstddef>

namespace ptl
{
#if __cplusplus >= 201703L
	using Byte = std::byte;
#else
	enum class Byte : unsigned char {};
#endif
}
