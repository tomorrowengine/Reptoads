












#pragma once


#include "../gtc/quaternion.hpp"
#include "../gtc/vec1.hpp"
#include "../ext/scalar_float_sized.hpp"
#include "../ext/scalar_int_sized.hpp"
#include "../ext/scalar_uint_sized.hpp"
#include "../detail/type_vec2.hpp"
#include "../detail/type_vec3.hpp"
#include "../detail/type_vec4.hpp"
#include "../detail/type_mat2x2.hpp"
#include "../detail/type_mat2x3.hpp"
#include "../detail/type_mat2x4.hpp"
#include "../detail/type_mat3x2.hpp"
#include "../detail/type_mat3x3.hpp"
#include "../detail/type_mat3x4.hpp"
#include "../detail/type_mat4x2.hpp"
#include "../detail/type_mat4x3.hpp"
#include "../detail/type_mat4x4.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_type_precision extension included")
#endif

namespace glm
{
	
	

	
	

	
	
	typedef detail::int8 lowp_int8;

	
	
	typedef detail::int16 lowp_int16;

	
	
	typedef detail::int32 lowp_int32;

	
	
	typedef detail::int64 lowp_int64;

	
	
	typedef detail::int8 lowp_int8_t;

	
	
	typedef detail::int16 lowp_int16_t;

	
	
	typedef detail::int32 lowp_int32_t;

	
	
	typedef detail::int64 lowp_int64_t;

	
	
	typedef detail::int8 lowp_i8;

	
	
	typedef detail::int16 lowp_i16;

	
	
	typedef detail::int32 lowp_i32;

	
	
	typedef detail::int64 lowp_i64;

	
	
	typedef detail::int8 mediump_int8;

	
	
	typedef detail::int16 mediump_int16;

	
	
	typedef detail::int32 mediump_int32;

	
	
	typedef detail::int64 mediump_int64;

	
	
	typedef detail::int8 mediump_int8_t;

	
	
	typedef detail::int16 mediump_int16_t;

	
	
	typedef detail::int32 mediump_int32_t;

	
	
	typedef detail::int64 mediump_int64_t;

	
	
	typedef detail::int8 mediump_i8;

	
	
	typedef detail::int16 mediump_i16;

	
	
	typedef detail::int32 mediump_i32;

	
	
	typedef detail::int64 mediump_i64;

	
	
	typedef detail::int8 highp_int8;

	
	
	typedef detail::int16 highp_int16;

	
	
	typedef detail::int32 highp_int32;

	
	
	typedef detail::int64 highp_int64;

	
	
	typedef detail::int8 highp_int8_t;

	
	
	typedef detail::int16 highp_int16_t;

	
	
	typedef detail::int32 highp_int32_t;

	
	
	typedef detail::int64 highp_int64_t;

	
	
	typedef detail::int8 highp_i8;

	
	
	typedef detail::int16 highp_i16;

	
	
	typedef detail::int32 highp_i32;

	
	
	typedef detail::int64 highp_i64;


#if GLM_HAS_EXTENDED_INTEGER_TYPE
	using std::int8_t;
	using std::int16_t;
	using std::int32_t;
	using std::int64_t;
#else
	
	
	typedef detail::int8 int8_t;

	
	
	typedef detail::int16 int16_t;

	
	
	typedef detail::int32 int32_t;

	
	
	typedef detail::int64 int64_t;
#endif

	
	
	typedef detail::int8 i8;

	
	
	typedef detail::int16 i16;

	
	
	typedef detail::int32 i32;

	
	
	typedef detail::int64 i64;



	
	
	typedef vec<1, i8, lowp> lowp_i8vec1;

	
	
	typedef vec<2, i8, lowp> lowp_i8vec2;

	
	
	typedef vec<3, i8, lowp> lowp_i8vec3;

	
	
	typedef vec<4, i8, lowp> lowp_i8vec4;


	
	
	typedef vec<1, i8, mediump> mediump_i8vec1;

	
	
	typedef vec<2, i8, mediump> mediump_i8vec2;

	
	
	typedef vec<3, i8, mediump> mediump_i8vec3;

	
	
	typedef vec<4, i8, mediump> mediump_i8vec4;


	
	
	typedef vec<1, i8, highp> highp_i8vec1;

	
	
	typedef vec<2, i8, highp> highp_i8vec2;

	
	
	typedef vec<3, i8, highp> highp_i8vec3;

	
	
	typedef vec<4, i8, highp> highp_i8vec4;



	
	
	typedef vec<1, i8, defaultp> i8vec1;

	
	
	typedef vec<2, i8, defaultp> i8vec2;

	
	
	typedef vec<3, i8, defaultp> i8vec3;

	
	
	typedef vec<4, i8, defaultp> i8vec4;





	
	
	typedef vec<1, i16, lowp>		lowp_i16vec1;

	
	
	typedef vec<2, i16, lowp>		lowp_i16vec2;

	
	
	typedef vec<3, i16, lowp>		lowp_i16vec3;

	
	
	typedef vec<4, i16, lowp>		lowp_i16vec4;


	
	
	typedef vec<1, i16, mediump>		mediump_i16vec1;

	
	
	typedef vec<2, i16, mediump>		mediump_i16vec2;

	
	
	typedef vec<3, i16, mediump>		mediump_i16vec3;

	
	
	typedef vec<4, i16, mediump>		mediump_i16vec4;


	
	
	typedef vec<1, i16, highp>		highp_i16vec1;

	
	
	typedef vec<2, i16, highp>		highp_i16vec2;

	
	
	typedef vec<3, i16, highp>		highp_i16vec3;

	
	
	typedef vec<4, i16, highp>		highp_i16vec4;




	
	
	typedef vec<1, i16, defaultp> i16vec1;

	
	
	typedef vec<2, i16, defaultp> i16vec2;

	
	
	typedef vec<3, i16, defaultp> i16vec3;

	
	
	typedef vec<4, i16, defaultp> i16vec4;



	
	
	typedef vec<1, i32, lowp>		lowp_i32vec1;

	
	
	typedef vec<2, i32, lowp>		lowp_i32vec2;

	
	
	typedef vec<3, i32, lowp>		lowp_i32vec3;

	
	
	typedef vec<4, i32, lowp>		lowp_i32vec4;


	
	
	typedef vec<1, i32, mediump>		mediump_i32vec1;

	
	
	typedef vec<2, i32, mediump>		mediump_i32vec2;

	
	
	typedef vec<3, i32, mediump>		mediump_i32vec3;

	
	
	typedef vec<4, i32, mediump>		mediump_i32vec4;


	
	
	typedef vec<1, i32, highp>		highp_i32vec1;

	
	
	typedef vec<2, i32, highp>		highp_i32vec2;

	
	
	typedef vec<3, i32, highp>		highp_i32vec3;

	
	
	typedef vec<4, i32, highp>		highp_i32vec4;


	
	
	typedef vec<1, i32, defaultp> i32vec1;

	
	
	typedef vec<2, i32, defaultp> i32vec2;

	
	
	typedef vec<3, i32, defaultp> i32vec3;

	
	
	typedef vec<4, i32, defaultp> i32vec4;




	
	
	typedef vec<1, i64, lowp>		lowp_i64vec1;

	
	
	typedef vec<2, i64, lowp>		lowp_i64vec2;

	
	
	typedef vec<3, i64, lowp>		lowp_i64vec3;

	
	
	typedef vec<4, i64, lowp>		lowp_i64vec4;


	
	
	typedef vec<1, i64, mediump>		mediump_i64vec1;

	
	
	typedef vec<2, i64, mediump>		mediump_i64vec2;

	
	
	typedef vec<3, i64, mediump>		mediump_i64vec3;

	
	
	typedef vec<4, i64, mediump>		mediump_i64vec4;


	
	
	typedef vec<1, i64, highp>		highp_i64vec1;

	
	
	typedef vec<2, i64, highp>		highp_i64vec2;

	
	
	typedef vec<3, i64, highp>		highp_i64vec3;

	
	
	typedef vec<4, i64, highp>		highp_i64vec4;


	
	
	typedef vec<1, i64, defaultp> i64vec1;

	
	
	typedef vec<2, i64, defaultp> i64vec2;

	
	
	typedef vec<3, i64, defaultp> i64vec3;

	
	
	typedef vec<4, i64, defaultp> i64vec4;


	
	

	
	
	typedef detail::uint8 lowp_uint8;

	
	
	typedef detail::uint16 lowp_uint16;

	
	
	typedef detail::uint32 lowp_uint32;

	
	
	typedef detail::uint64 lowp_uint64;

	
	
	typedef detail::uint8 lowp_uint8_t;

	
	
	typedef detail::uint16 lowp_uint16_t;

	
	
	typedef detail::uint32 lowp_uint32_t;

	
	
	typedef detail::uint64 lowp_uint64_t;

	
	
	typedef detail::uint8 lowp_u8;

	
	
	typedef detail::uint16 lowp_u16;

	
	
	typedef detail::uint32 lowp_u32;

	
	
	typedef detail::uint64 lowp_u64;

	
	
	typedef detail::uint8 mediump_uint8;

	
	
	typedef detail::uint16 mediump_uint16;

	
	
	typedef detail::uint32 mediump_uint32;

	
	
	typedef detail::uint64 mediump_uint64;

	
	
	typedef detail::uint8 mediump_uint8_t;

	
	
	typedef detail::uint16 mediump_uint16_t;

	
	
	typedef detail::uint32 mediump_uint32_t;

	
	
	typedef detail::uint64 mediump_uint64_t;

	
	
	typedef detail::uint8 mediump_u8;

	
	
	typedef detail::uint16 mediump_u16;

	
	
	typedef detail::uint32 mediump_u32;

	
	
	typedef detail::uint64 mediump_u64;

	
	
	typedef detail::uint8 highp_uint8;

	
	
	typedef detail::uint16 highp_uint16;

	
	
	typedef detail::uint32 highp_uint32;

	
	
	typedef detail::uint64 highp_uint64;

	
	
	typedef detail::uint8 highp_uint8_t;

	
	
	typedef detail::uint16 highp_uint16_t;

	
	
	typedef detail::uint32 highp_uint32_t;

	
	
	typedef detail::uint64 highp_uint64_t;

	
	
	typedef detail::uint8 highp_u8;

	
	
	typedef detail::uint16 highp_u16;

	
	
	typedef detail::uint32 highp_u32;

	
	
	typedef detail::uint64 highp_u64;

#if GLM_HAS_EXTENDED_INTEGER_TYPE
	using std::uint8_t;
	using std::uint16_t;
	using std::uint32_t;
	using std::uint64_t;
#else
	
	
	typedef detail::uint8 uint8_t;

	
	
	typedef detail::uint16 uint16_t;

	
	
	typedef detail::uint32 uint32_t;

	
	
	typedef detail::uint64 uint64_t;
#endif

	
	
	typedef detail::uint8 u8;

	
	
	typedef detail::uint16 u16;

	
	
	typedef detail::uint32 u32;

	
	
	typedef detail::uint64 u64;





	
	

	
	
	typedef float32 lowp_float32;

	
	
	typedef float64 lowp_float64;

	
	
	typedef float32 lowp_float32_t;

	
	
	typedef float64 lowp_float64_t;

	
	
	typedef float32 lowp_f32;

	
	
	typedef float64 lowp_f64;

	
	
	typedef float32 lowp_float32;

	
	
	typedef float64 lowp_float64;

	
	
	typedef float32 lowp_float32_t;

	
	
	typedef float64 lowp_float64_t;

	
	
	typedef float32 lowp_f32;

	
	
	typedef float64 lowp_f64;


	
	
	typedef float32 lowp_float32;

	
	
	typedef float64 lowp_float64;

	
	
	typedef float32 lowp_float32_t;

	
	
	typedef float64 lowp_float64_t;

	
	
	typedef float32 lowp_f32;

	
	
	typedef float64 lowp_f64;


	
	
	typedef float32 mediump_float32;

	
	
	typedef float64 mediump_float64;

	
	
	typedef float32 mediump_float32_t;

	
	
	typedef float64 mediump_float64_t;

	
	
	typedef float32 mediump_f32;

	
	
	typedef float64 mediump_f64;


	
	
	typedef float32 highp_float32;

	
	
	typedef float64 highp_float64;

	
	
	typedef float32 highp_float32_t;

	
	
	typedef float64 highp_float64_t;

	
	
	typedef float32 highp_f32;

	
	
	typedef float64 highp_f64;


#if(defined(GLM_PRECISION_LOWP_FLOAT))
	
	
	typedef lowp_float32_t float32_t;

	
	
	typedef lowp_float64_t float64_t;

	
	
	typedef lowp_f32 f32;

	
	
	typedef lowp_f64 f64;

#elif(defined(GLM_PRECISION_MEDIUMP_FLOAT))
	
	
	typedef mediump_float32 float32_t;

	
	
	typedef mediump_float64 float64_t;

	
	
	typedef mediump_float32 f32;

	
	
	typedef mediump_float64 f64;

#else

	
	
	typedef highp_float32_t float32_t;

	
	
	typedef highp_float64_t float64_t;

	
	
	typedef highp_float32_t f32;

	
	
	typedef highp_float64_t f64;
#endif


	
	
	typedef vec<1, float, lowp> lowp_fvec1;

	
	
	typedef vec<2, float, lowp> lowp_fvec2;

	
	
	typedef vec<3, float, lowp> lowp_fvec3;

	
	
	typedef vec<4, float, lowp> lowp_fvec4;


	
	
	typedef vec<1, float, mediump> mediump_fvec1;

	
	
	typedef vec<2, float, mediump> mediump_fvec2;

	
	
	typedef vec<3, float, mediump> mediump_fvec3;

	
	
	typedef vec<4, float, mediump> mediump_fvec4;


	
	
	typedef vec<1, float, highp> highp_fvec1;

	
	
	typedef vec<2, float, highp> highp_fvec2;

	
	
	typedef vec<3, float, highp> highp_fvec3;

	
	
	typedef vec<4, float, highp> highp_fvec4;


	
	
	typedef vec<1, f32, lowp> lowp_f32vec1;

	
	
	typedef vec<2, f32, lowp> lowp_f32vec2;

	
	
	typedef vec<3, f32, lowp> lowp_f32vec3;

	
	
	typedef vec<4, f32, lowp> lowp_f32vec4;

	
	
	typedef vec<1, f32, mediump> mediump_f32vec1;

	
	
	typedef vec<2, f32, mediump> mediump_f32vec2;

	
	
	typedef vec<3, f32, mediump> mediump_f32vec3;

	
	
	typedef vec<4, f32, mediump> mediump_f32vec4;

	
	
	typedef vec<1, f32, highp> highp_f32vec1;

	
	
	typedef vec<2, f32, highp> highp_f32vec2;

	
	
	typedef vec<3, f32, highp> highp_f32vec3;

	
	
	typedef vec<4, f32, highp> highp_f32vec4;


	
	
	typedef vec<1, f64, lowp> lowp_f64vec1;

	
	
	typedef vec<2, f64, lowp> lowp_f64vec2;

	
	
	typedef vec<3, f64, lowp> lowp_f64vec3;

	
	
	typedef vec<4, f64, lowp> lowp_f64vec4;

	
	
	typedef vec<1, f64, mediump> mediump_f64vec1;

	
	
	typedef vec<2, f64, mediump> mediump_f64vec2;

	
	
	typedef vec<3, f64, mediump> mediump_f64vec3;

	
	
	typedef vec<4, f64, mediump> mediump_f64vec4;

	
	
	typedef vec<1, f64, highp> highp_f64vec1;

	
	
	typedef vec<2, f64, highp> highp_f64vec2;

	
	
	typedef vec<3, f64, highp> highp_f64vec3;

	
	
	typedef vec<4, f64, highp> highp_f64vec4;



	
	

	
	
	

	
	
	typedef mat<2, 2, f32, lowp> lowp_fmat2x2;

	
	
	typedef mat<2, 3, f32, lowp> lowp_fmat2x3;

	
	
	typedef mat<2, 4, f32, lowp> lowp_fmat2x4;

	
	
	typedef mat<3, 2, f32, lowp> lowp_fmat3x2;

	
	
	typedef mat<3, 3, f32, lowp> lowp_fmat3x3;

	
	
	typedef mat<3, 4, f32, lowp> lowp_fmat3x4;

	
	
	typedef mat<4, 2, f32, lowp> lowp_fmat4x2;

	
	
	typedef mat<4, 3, f32, lowp> lowp_fmat4x3;

	
	
	typedef mat<4, 4, f32, lowp> lowp_fmat4x4;

	
	
	

	
	
	typedef lowp_fmat2x2 lowp_fmat2;

	
	
	typedef lowp_fmat3x3 lowp_fmat3;

	
	
	typedef lowp_fmat4x4 lowp_fmat4;


	
	
	

	
	
	typedef mat<2, 2, f32, mediump> mediump_fmat2x2;

	
	
	typedef mat<2, 3, f32, mediump> mediump_fmat2x3;

	
	
	typedef mat<2, 4, f32, mediump> mediump_fmat2x4;

	
	
	typedef mat<3, 2, f32, mediump> mediump_fmat3x2;

	
	
	typedef mat<3, 3, f32, mediump> mediump_fmat3x3;

	
	
	typedef mat<3, 4, f32, mediump> mediump_fmat3x4;

	
	
	typedef mat<4, 2, f32, mediump> mediump_fmat4x2;

	
	
	typedef mat<4, 3, f32, mediump> mediump_fmat4x3;

	
	
	typedef mat<4, 4, f32, mediump> mediump_fmat4x4;

	
	
	

	
	
	typedef mediump_fmat2x2 mediump_fmat2;

	
	
	typedef mediump_fmat3x3 mediump_fmat3;

	
	
	typedef mediump_fmat4x4 mediump_fmat4;


	
	
	

	
	
	typedef mat<2, 2, f32, highp> highp_fmat2x2;

	
	
	typedef mat<2, 3, f32, highp> highp_fmat2x3;

	
	
	typedef mat<2, 4, f32, highp> highp_fmat2x4;

	
	
	typedef mat<3, 2, f32, highp> highp_fmat3x2;

	
	
	typedef mat<3, 3, f32, highp> highp_fmat3x3;

	
	
	typedef mat<3, 4, f32, highp> highp_fmat3x4;

	
	
	typedef mat<4, 2, f32, highp> highp_fmat4x2;

	
	
	typedef mat<4, 3, f32, highp> highp_fmat4x3;

	
	
	typedef mat<4, 4, f32, highp> highp_fmat4x4;

	
	
	

	
	
	typedef highp_fmat2x2 highp_fmat2;

	
	
	typedef highp_fmat3x3 highp_fmat3;

	
	
	typedef highp_fmat4x4 highp_fmat4;


	
	
	

	
	
	typedef mat<2, 2, f32, lowp> lowp_f32mat2x2;

	
	
	typedef mat<2, 3, f32, lowp> lowp_f32mat2x3;

	
	
	typedef mat<2, 4, f32, lowp> lowp_f32mat2x4;

	
	
	typedef mat<3, 2, f32, lowp> lowp_f32mat3x2;

	
	
	typedef mat<3, 3, f32, lowp> lowp_f32mat3x3;

	
	
	typedef mat<3, 4, f32, lowp> lowp_f32mat3x4;

	
	
	typedef mat<4, 2, f32, lowp> lowp_f32mat4x2;

	
	
	typedef mat<4, 3, f32, lowp> lowp_f32mat4x3;

	
	
	typedef mat<4, 4, f32, lowp> lowp_f32mat4x4;

	
	
	

	
	
	typedef lowp_f32mat2x2 lowp_f32mat2;

	
	
	typedef lowp_f32mat3x3 lowp_f32mat3;

	
	
	typedef lowp_f32mat4x4 lowp_f32mat4;


	
	
	

	
	
	typedef mat<2, 2, f32, mediump> mediump_f32mat2x2;

	
	
	typedef mat<2, 3, f32, mediump> mediump_f32mat2x3;

	
	
	typedef mat<2, 4, f32, mediump> mediump_f32mat2x4;

	
	
	typedef mat<3, 2, f32, mediump> mediump_f32mat3x2;

	
	
	typedef mat<3, 3, f32, mediump> mediump_f32mat3x3;

	
	
	typedef mat<3, 4, f32, mediump> mediump_f32mat3x4;

	
	
	typedef mat<4, 2, f32, mediump> mediump_f32mat4x2;

	
	
	typedef mat<4, 3, f32, mediump> mediump_f32mat4x3;

	
	
	typedef mat<4, 4, f32, mediump> mediump_f32mat4x4;

	
	
	

	
	
	typedef mediump_f32mat2x2 mediump_f32mat2;

	
	
	typedef mediump_f32mat3x3 mediump_f32mat3;

	
	
	typedef mediump_f32mat4x4 mediump_f32mat4;


	
	
	

	
	
	typedef mat<2, 2, f32, highp> highp_f32mat2x2;

	
	
	typedef mat<2, 3, f32, highp> highp_f32mat2x3;

	
	
	typedef mat<2, 4, f32, highp> highp_f32mat2x4;

	
	
	typedef mat<3, 2, f32, highp> highp_f32mat3x2;

	
	
	typedef mat<3, 3, f32, highp> highp_f32mat3x3;

	
	
	typedef mat<3, 4, f32, highp> highp_f32mat3x4;

	
	
	typedef mat<4, 2, f32, highp> highp_f32mat4x2;

	
	
	typedef mat<4, 3, f32, highp> highp_f32mat4x3;

	
	
	typedef mat<4, 4, f32, highp> highp_f32mat4x4;

	
	
	

	
	
	typedef highp_f32mat2x2 highp_f32mat2;

	
	
	typedef highp_f32mat3x3 highp_f32mat3;

	
	
	typedef highp_f32mat4x4 highp_f32mat4;


	
	
	

	
	
	typedef mat<2, 2, f64, lowp> lowp_f64mat2x2;

	
	
	typedef mat<2, 3, f64, lowp> lowp_f64mat2x3;

	
	
	typedef mat<2, 4, f64, lowp> lowp_f64mat2x4;

	
	
	typedef mat<3, 2, f64, lowp> lowp_f64mat3x2;

	
	
	typedef mat<3, 3, f64, lowp> lowp_f64mat3x3;

	
	
	typedef mat<3, 4, f64, lowp> lowp_f64mat3x4;

	
	
	typedef mat<4, 2, f64, lowp> lowp_f64mat4x2;

	
	
	typedef mat<4, 3, f64, lowp> lowp_f64mat4x3;

	
	
	typedef mat<4, 4, f64, lowp> lowp_f64mat4x4;

	
	
	

	
	
	typedef lowp_f64mat2x2 lowp_f64mat2;

	
	
	typedef lowp_f64mat3x3 lowp_f64mat3;

	
	
	typedef lowp_f64mat4x4 lowp_f64mat4;


	
	
	

	
	
	typedef mat<2, 2, f64, mediump> mediump_f64mat2x2;

	
	
	typedef mat<2, 3, f64, mediump> mediump_f64mat2x3;

	
	
	typedef mat<2, 4, f64, mediump> mediump_f64mat2x4;

	
	
	typedef mat<3, 2, f64, mediump> mediump_f64mat3x2;

	
	
	typedef mat<3, 3, f64, mediump> mediump_f64mat3x3;

	
	
	typedef mat<3, 4, f64, mediump> mediump_f64mat3x4;

	
	
	typedef mat<4, 2, f64, mediump> mediump_f64mat4x2;

	
	
	typedef mat<4, 3, f64, mediump> mediump_f64mat4x3;

	
	
	typedef mat<4, 4, f64, mediump> mediump_f64mat4x4;

	
	
	

	
	
	typedef mediump_f64mat2x2 mediump_f64mat2;

	
	
	typedef mediump_f64mat3x3 mediump_f64mat3;

	
	
	typedef mediump_f64mat4x4 mediump_f64mat4;

	
	
	

	
	
	typedef mat<2, 2, f64, highp> highp_f64mat2x2;

	
	
	typedef mat<2, 3, f64, highp> highp_f64mat2x3;

	
	
	typedef mat<2, 4, f64, highp> highp_f64mat2x4;

	
	
	typedef mat<3, 2, f64, highp> highp_f64mat3x2;

	
	
	typedef mat<3, 3, f64, highp> highp_f64mat3x3;

	
	
	typedef mat<3, 4, f64, highp> highp_f64mat3x4;

	
	
	typedef mat<4, 2, f64, highp> highp_f64mat4x2;

	
	
	typedef mat<4, 3, f64, highp> highp_f64mat4x3;

	
	
	typedef mat<4, 4, f64, highp> highp_f64mat4x4;

	
	
	

	
	
	typedef highp_f64mat2x2 highp_f64mat2;

	
	
	typedef highp_f64mat3x3 highp_f64mat3;

	
	
	typedef highp_f64mat4x4 highp_f64mat4;




	
	
	typedef vec<1, u8, lowp> lowp_u8vec1;

	
	
	typedef vec<2, u8, lowp> lowp_u8vec2;

	
	
	typedef vec<3, u8, lowp> lowp_u8vec3;

	
	
	typedef vec<4, u8, lowp> lowp_u8vec4;


	
	
	typedef vec<1, u8, mediump> mediump_u8vec1;

	
	
	typedef vec<2, u8, mediump> mediump_u8vec2;

	
	
	typedef vec<3, u8, mediump> mediump_u8vec3;

	
	
	typedef vec<4, u8, mediump> mediump_u8vec4;


	
	
	typedef vec<1, u8, highp> highp_u8vec1;

	
	
	typedef vec<2, u8, highp> highp_u8vec2;

	
	
	typedef vec<3, u8, highp> highp_u8vec3;

	
	
	typedef vec<4, u8, highp> highp_u8vec4;



	
	
	typedef vec<1, u8, defaultp> u8vec1;

	
	
	typedef vec<2, u8, defaultp> u8vec2;

	
	
	typedef vec<3, u8, defaultp> u8vec3;

	
	
	typedef vec<4, u8, defaultp> u8vec4;




	
	
	typedef vec<1, u16, lowp>		lowp_u16vec1;

	
	
	typedef vec<2, u16, lowp>		lowp_u16vec2;

	
	
	typedef vec<3, u16, lowp>		lowp_u16vec3;

	
	
	typedef vec<4, u16, lowp>		lowp_u16vec4;


	
	
	typedef vec<1, u16, mediump>		mediump_u16vec1;

	
	
	typedef vec<2, u16, mediump>		mediump_u16vec2;

	
	
	typedef vec<3, u16, mediump>		mediump_u16vec3;

	
	
	typedef vec<4, u16, mediump>		mediump_u16vec4;


	
	
	typedef vec<1, u16, highp>		highp_u16vec1;

	
	
	typedef vec<2, u16, highp>		highp_u16vec2;

	
	
	typedef vec<3, u16, highp>		highp_u16vec3;

	
	
	typedef vec<4, u16, highp>		highp_u16vec4;




	
	
	typedef vec<1, u16, defaultp> u16vec1;

	
	
	typedef vec<2, u16, defaultp> u16vec2;

	
	
	typedef vec<3, u16, defaultp> u16vec3;

	
	
	typedef vec<4, u16, defaultp> u16vec4;



	
	
	typedef vec<1, u32, lowp>		lowp_u32vec1;

	
	
	typedef vec<2, u32, lowp>		lowp_u32vec2;

	
	
	typedef vec<3, u32, lowp>		lowp_u32vec3;

	
	
	typedef vec<4, u32, lowp>		lowp_u32vec4;


	
	
	typedef vec<1, u32, mediump>		mediump_u32vec1;

	
	
	typedef vec<2, u32, mediump>		mediump_u32vec2;

	
	
	typedef vec<3, u32, mediump>		mediump_u32vec3;

	
	
	typedef vec<4, u32, mediump>		mediump_u32vec4;


	
	
	typedef vec<1, u32, highp>		highp_u32vec1;

	
	
	typedef vec<2, u32, highp>		highp_u32vec2;

	
	
	typedef vec<3, u32, highp>		highp_u32vec3;

	
	
	typedef vec<4, u32, highp>		highp_u32vec4;



	
	
	typedef vec<1, u32, defaultp> u32vec1;

	
	
	typedef vec<2, u32, defaultp> u32vec2;

	
	
	typedef vec<3, u32, defaultp> u32vec3;

	
	
	typedef vec<4, u32, defaultp> u32vec4;




	
	
	typedef vec<1, u64, lowp>		lowp_u64vec1;

	
	
	typedef vec<2, u64, lowp>		lowp_u64vec2;

	
	
	typedef vec<3, u64, lowp>		lowp_u64vec3;

	
	
	typedef vec<4, u64, lowp>		lowp_u64vec4;


	
	
	typedef vec<1, u64, mediump>		mediump_u64vec1;

	
	
	typedef vec<2, u64, mediump>		mediump_u64vec2;

	
	
	typedef vec<3, u64, mediump>		mediump_u64vec3;

	
	
	typedef vec<4, u64, mediump>		mediump_u64vec4;


	
	
	typedef vec<1, u64, highp>		highp_u64vec1;

	
	
	typedef vec<2, u64, highp>		highp_u64vec2;

	
	
	typedef vec<3, u64, highp>		highp_u64vec3;

	
	
	typedef vec<4, u64, highp>		highp_u64vec4;




	
	
	typedef vec<1, u64, defaultp> u64vec1;

	
	
	typedef vec<2, u64, defaultp> u64vec2;

	
	
	typedef vec<3, u64, defaultp> u64vec3;

	
	
	typedef vec<4, u64, defaultp> u64vec4;


	
	

	
	
	typedef float32 float32_t;

	
	
	typedef float32 f32;

#	ifndef GLM_FORCE_SINGLE_ONLY

		
		
		typedef float64 float64_t;

		
		
		typedef float64 f64;
#	endif

	
	
	typedef vec<1, float, defaultp> fvec1;

	
	
	typedef vec<2, float, defaultp> fvec2;

	
	
	typedef vec<3, float, defaultp> fvec3;

	
	
	typedef vec<4, float, defaultp> fvec4;


	
	
	typedef vec<1, f32, defaultp> f32vec1;

	
	
	typedef vec<2, f32, defaultp> f32vec2;

	
	
	typedef vec<3, f32, defaultp> f32vec3;

	
	
	typedef vec<4, f32, defaultp> f32vec4;

#	ifndef GLM_FORCE_SINGLE_ONLY
		
		
		typedef vec<1, f64, defaultp> f64vec1;

		
		
		typedef vec<2, f64, defaultp> f64vec2;

		
		
		typedef vec<3, f64, defaultp> f64vec3;

		
		
		typedef vec<4, f64, defaultp> f64vec4;
#	endif


	
	

	
	
	

	
	
	typedef mat<2, 2, f32, defaultp> fmat2;

	
	
	typedef mat<3, 3, f32, defaultp> fmat3;

	
	
	typedef mat<4, 4, f32, defaultp> fmat4;


	
	
	

	
	
	typedef mat<2, 2, f32, defaultp> fmat2x2;

	
	
	typedef mat<2, 3, f32, defaultp> fmat2x3;

	
	
	typedef mat<2, 4, f32, defaultp> fmat2x4;

	
	
	typedef mat<3, 2, f32, defaultp> fmat3x2;

	
	
	typedef mat<3, 3, f32, defaultp> fmat3x3;

	
	
	typedef mat<3, 4, f32, defaultp> fmat3x4;

	
	
	typedef mat<4, 2, f32, defaultp> fmat4x2;

	
	
	typedef mat<4, 3, f32, defaultp> fmat4x3;

	
	
	typedef mat<4, 4, f32, defaultp> fmat4x4;


	
	
	

	
	
	typedef mat<2, 2, f32, defaultp> f32mat2;

	
	
	typedef mat<3, 3, f32, defaultp> f32mat3;

	
	
	typedef mat<4, 4, f32, defaultp> f32mat4;


	
	
	

	
	
	typedef mat<2, 2, f32, defaultp> f32mat2x2;

	
	
	typedef mat<2, 3, f32, defaultp> f32mat2x3;

	
	
	typedef mat<2, 4, f32, defaultp> f32mat2x4;

	
	
	typedef mat<3, 2, f32, defaultp> f32mat3x2;

	
	
	typedef mat<3, 3, f32, defaultp> f32mat3x3;

	
	
	typedef mat<3, 4, f32, defaultp> f32mat3x4;

	
	
	typedef mat<4, 2, f32, defaultp> f32mat4x2;

	
	
	typedef mat<4, 3, f32, defaultp> f32mat4x3;

	
	
	typedef mat<4, 4, f32, defaultp> f32mat4x4;


#	ifndef GLM_FORCE_SINGLE_ONLY

	
	
	

	
	
	typedef mat<2, 2, f64, defaultp> f64mat2;

	
	
	typedef mat<3, 3, f64, defaultp> f64mat3;

	
	
	typedef mat<4, 4, f64, defaultp> f64mat4;


	
	
	

	
	
	typedef mat<2, 2, f64, defaultp> f64mat2x2;

	
	
	typedef mat<2, 3, f64, defaultp> f64mat2x3;

	
	
	typedef mat<2, 4, f64, defaultp> f64mat2x4;

	
	
	typedef mat<3, 2, f64, defaultp> f64mat3x2;

	
	
	typedef mat<3, 3, f64, defaultp> f64mat3x3;

	
	
	typedef mat<3, 4, f64, defaultp> f64mat3x4;

	
	
	typedef mat<4, 2, f64, defaultp> f64mat4x2;

	
	
	typedef mat<4, 3, f64, defaultp> f64mat4x3;

	
	
	typedef mat<4, 4, f64, defaultp> f64mat4x4;

#	endif

	
	

	
	
	typedef qua<f32, defaultp> f32quat;

	
	
	typedef qua<f32, lowp> lowp_f32quat;

	
	
	typedef qua<f64, lowp> lowp_f64quat;

	
	
	typedef qua<f32, mediump> mediump_f32quat;

#	ifndef GLM_FORCE_SINGLE_ONLY

	
	
	typedef qua<f64, mediump> mediump_f64quat;

	
	
	typedef qua<f32, highp> highp_f32quat;

	
	
	typedef qua<f64, highp> highp_f64quat;

	
	
	typedef qua<f64, defaultp> f64quat;

#	endif

	
}

#include "type_precision.inl"
