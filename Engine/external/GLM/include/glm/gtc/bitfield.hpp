












#include "../detail/setup.hpp"

#pragma once


#include "../ext/scalar_int_sized.hpp"
#include "../ext/scalar_uint_sized.hpp"
#include "../detail/qualifier.hpp"
#include "../detail/_vectorize.hpp"
#include "type_precision.hpp"
#include <limits>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_bitfield extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL genIUType mask(genIUType Bits);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> mask(vec<L, T, Q> const& v);

	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL genIUType bitfieldRotateRight(genIUType In, int Shift);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> bitfieldRotateRight(vec<L, T, Q> const& In, int Shift);

	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL genIUType bitfieldRotateLeft(genIUType In, int Shift);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> bitfieldRotateLeft(vec<L, T, Q> const& In, int Shift);

	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL genIUType bitfieldFillOne(genIUType Value, int FirstBit, int BitCount);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> bitfieldFillOne(vec<L, T, Q> const& Value, int FirstBit, int BitCount);

	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL genIUType bitfieldFillZero(genIUType Value, int FirstBit, int BitCount);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> bitfieldFillZero(vec<L, T, Q> const& Value, int FirstBit, int BitCount);

	
	
	
	
	
	GLM_FUNC_DECL int16 bitfieldInterleave(int8 x, int8 y);

	
	
	
	
	
	GLM_FUNC_DECL uint16 bitfieldInterleave(uint8 x, uint8 y);

	
	
	
	
	
	GLM_FUNC_DECL uint16 bitfieldInterleave(u8vec2 const& v);

	
	
	
	GLM_FUNC_DECL glm::u8vec2 bitfieldDeinterleave(glm::uint16 x);

	
	
	
	
	
	GLM_FUNC_DECL int32 bitfieldInterleave(int16 x, int16 y);

	
	
	
	
	
	GLM_FUNC_DECL uint32 bitfieldInterleave(uint16 x, uint16 y);

	
	
	
	
	
	GLM_FUNC_DECL uint32 bitfieldInterleave(u16vec2 const& v);

	
	
	
	GLM_FUNC_DECL glm::u16vec2 bitfieldDeinterleave(glm::uint32 x);

	
	
	
	
	
	GLM_FUNC_DECL int64 bitfieldInterleave(int32 x, int32 y);

	
	
	
	
	
	GLM_FUNC_DECL uint64 bitfieldInterleave(uint32 x, uint32 y);

	
	
	
	
	
	GLM_FUNC_DECL uint64 bitfieldInterleave(u32vec2 const& v);

	
	
	
	GLM_FUNC_DECL glm::u32vec2 bitfieldDeinterleave(glm::uint64 x);

	
	
	
	
	
	GLM_FUNC_DECL int32 bitfieldInterleave(int8 x, int8 y, int8 z);

	
	
	
	
	
	GLM_FUNC_DECL uint32 bitfieldInterleave(uint8 x, uint8 y, uint8 z);

	
	
	
	
	
	GLM_FUNC_DECL int64 bitfieldInterleave(int16 x, int16 y, int16 z);

	
	
	
	
	
	GLM_FUNC_DECL uint64 bitfieldInterleave(uint16 x, uint16 y, uint16 z);

	
	
	
	
	
	GLM_FUNC_DECL int64 bitfieldInterleave(int32 x, int32 y, int32 z);

	
	
	
	
	
	GLM_FUNC_DECL uint64 bitfieldInterleave(uint32 x, uint32 y, uint32 z);

	
	
	
	
	
	GLM_FUNC_DECL int32 bitfieldInterleave(int8 x, int8 y, int8 z, int8 w);

	
	
	
	
	
	GLM_FUNC_DECL uint32 bitfieldInterleave(uint8 x, uint8 y, uint8 z, uint8 w);

	
	
	
	
	
	GLM_FUNC_DECL int64 bitfieldInterleave(int16 x, int16 y, int16 z, int16 w);

	
	
	
	
	
	GLM_FUNC_DECL uint64 bitfieldInterleave(uint16 x, uint16 y, uint16 z, uint16 w);

	
} 

#include "bitfield.inl"
