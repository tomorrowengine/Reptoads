












#pragma once


#include "type_precision.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_packing extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint8 packUnorm1x8(float v);

	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL float unpackUnorm1x8(uint8 p);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint16 packUnorm2x8(vec2 const& v);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL vec2 unpackUnorm2x8(uint16 p);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint8 packSnorm1x8(float s);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL float unpackSnorm1x8(uint8 p);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint16 packSnorm2x8(vec2 const& v);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL vec2 unpackSnorm2x8(uint16 p);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint16 packUnorm1x16(float v);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL float unpackUnorm1x16(uint16 p);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint64 packUnorm4x16(vec4 const& v);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL vec4 unpackUnorm4x16(uint64 p);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint16 packSnorm1x16(float v);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL float unpackSnorm1x16(uint16 p);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint64 packSnorm4x16(vec4 const& v);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL vec4 unpackSnorm4x16(uint64 p);

	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint16 packHalf1x16(float v);

	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL float unpackHalf1x16(uint16 v);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint64 packHalf4x16(vec4 const& v);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL vec4 unpackHalf4x16(uint64 p);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint32 packI3x10_1x2(ivec4 const& v);

	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL ivec4 unpackI3x10_1x2(uint32 p);

	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint32 packU3x10_1x2(uvec4 const& v);

	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uvec4 unpackU3x10_1x2(uint32 p);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint32 packSnorm3x10_1x2(vec4 const& v);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL vec4 unpackSnorm3x10_1x2(uint32 p);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint32 packUnorm3x10_1x2(vec4 const& v);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL vec4 unpackUnorm3x10_1x2(uint32 p);

	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint32 packF2x11_1x10(vec3 const& v);

	
	
	
	
	
	
	
	
	GLM_FUNC_DECL vec3 unpackF2x11_1x10(uint32 p);


	
	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL uint32 packF3x9_E1x5(vec3 const& v);

	
	
	
	
	
	
	
	
	
	
	GLM_FUNC_DECL vec3 unpackF3x9_E1x5(uint32 p);

	
	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> packRGBM(vec<3, T, Q> const& rgb);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> unpackRGBM(vec<4, T, Q> const& rgbm);

	
	
	
	
	
	
	
	
	template<length_t L, qualifier Q>
	GLM_FUNC_DECL vec<L, uint16, Q> packHalf(vec<L, float, Q> const& v);

	
	
	
	
	
	
	
	template<length_t L, qualifier Q>
	GLM_FUNC_DECL vec<L, float, Q> unpackHalf(vec<L, uint16, Q> const& p);

	
	
	
	
	template<typename uintType, length_t L, typename floatType, qualifier Q>
	GLM_FUNC_DECL vec<L, uintType, Q> packUnorm(vec<L, floatType, Q> const& v);

	
	
	
	
	template<typename floatType, length_t L, typename uintType, qualifier Q>
	GLM_FUNC_DECL vec<L, floatType, Q> unpackUnorm(vec<L, uintType, Q> const& v);

	
	
	
	
	template<typename intType, length_t L, typename floatType, qualifier Q>
	GLM_FUNC_DECL vec<L, intType, Q> packSnorm(vec<L, floatType, Q> const& v);

	
	
	
	
	template<typename floatType, length_t L, typename intType, qualifier Q>
	GLM_FUNC_DECL vec<L, floatType, Q> unpackSnorm(vec<L, intType, Q> const& v);

	
	
	
	
	GLM_FUNC_DECL uint8 packUnorm2x4(vec2 const& v);

	
	
	
	
	GLM_FUNC_DECL vec2 unpackUnorm2x4(uint8 p);

	
	
	
	
	GLM_FUNC_DECL uint16 packUnorm4x4(vec4 const& v);

	
	
	
	
	GLM_FUNC_DECL vec4 unpackUnorm4x4(uint16 p);

	
	
	
	
	GLM_FUNC_DECL uint16 packUnorm1x5_1x6_1x5(vec3 const& v);

	
	
	
	
	GLM_FUNC_DECL vec3 unpackUnorm1x5_1x6_1x5(uint16 p);

	
	
	
	
	GLM_FUNC_DECL uint16 packUnorm3x5_1x1(vec4 const& v);

	
	
	
	
	GLM_FUNC_DECL vec4 unpackUnorm3x5_1x1(uint16 p);

	
	
	
	
	GLM_FUNC_DECL uint8 packUnorm2x3_1x2(vec3 const& v);

	
	
	
	
	GLM_FUNC_DECL vec3 unpackUnorm2x3_1x2(uint8 p);



	
	
	
	
	GLM_FUNC_DECL int16 packInt2x8(i8vec2 const& v);

	
	
	
	
	GLM_FUNC_DECL i8vec2 unpackInt2x8(int16 p);

	
	
	
	
	GLM_FUNC_DECL uint16 packUint2x8(u8vec2 const& v);

	
	
	
	
	GLM_FUNC_DECL u8vec2 unpackUint2x8(uint16 p);

	
	
	
	
	GLM_FUNC_DECL int32 packInt4x8(i8vec4 const& v);

	
	
	
	
	GLM_FUNC_DECL i8vec4 unpackInt4x8(int32 p);

	
	
	
	
	GLM_FUNC_DECL uint32 packUint4x8(u8vec4 const& v);

	
	
	
	
	GLM_FUNC_DECL u8vec4 unpackUint4x8(uint32 p);

	
	
	
	
	GLM_FUNC_DECL int packInt2x16(i16vec2 const& v);

	
	
	
	
	GLM_FUNC_DECL i16vec2 unpackInt2x16(int p);

	
	
	
	
	GLM_FUNC_DECL int64 packInt4x16(i16vec4 const& v);

	
	
	
	
	GLM_FUNC_DECL i16vec4 unpackInt4x16(int64 p);

	
	
	
	
	GLM_FUNC_DECL uint packUint2x16(u16vec2 const& v);

	
	
	
	
	GLM_FUNC_DECL u16vec2 unpackUint2x16(uint p);

	
	
	
	
	GLM_FUNC_DECL uint64 packUint4x16(u16vec4 const& v);

	
	
	
	
	GLM_FUNC_DECL u16vec4 unpackUint4x16(uint64 p);

	
	
	
	
	GLM_FUNC_DECL int64 packInt2x32(i32vec2 const& v);

	
	
	
	
	GLM_FUNC_DECL i32vec2 unpackInt2x32(int64 p);

	
	
	
	
	GLM_FUNC_DECL uint64 packUint2x32(u32vec2 const& v);

	
	
	
	
	GLM_FUNC_DECL u32vec2 unpackUint2x32(uint64 p);


	
}

#include "packing.inl"
