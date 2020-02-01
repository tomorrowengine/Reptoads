#pragma once


#define UNUSED(variable) (void)variable

#define MAKE_RGBA(r, g, b, a) (static_cast<uint32_t>(r) | (static_cast<uint32_t>(g) << 8) | (static_cast<uint32_t>(b) << 16) | (static_cast<uint32_t>(a) << 24))

inline void EmptyFunction(){}

#ifdef _MSC_VER
template<typename T>
void StaticAssertPrint()
{
	static_assert(false, "");
}

#endif
