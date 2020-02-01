











#pragma once

#if !GLM_CONFIG_ANONYMOUS_STRUCT
#	error "GLM: Aligned gentypes require to enable C++ language extensions."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
# pragma message("GLM: GLM_GTC_type_aligned extension included")
#endif

#include "../mat4x4.hpp"
#include "../mat4x3.hpp"
#include "../mat4x2.hpp"
#include "../mat3x4.hpp"
#include "../mat3x3.hpp"
#include "../mat3x2.hpp"
#include "../mat2x4.hpp"
#include "../mat2x3.hpp"
#include "../mat2x2.hpp"
#include "../gtc/vec1.hpp"
#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"

namespace glm
{
	
	

	

	
	typedef vec<1, float, aligned_highp>	aligned_highp_vec1;

	
	typedef vec<1, float, aligned_mediump>	aligned_mediump_vec1;

	
	typedef vec<1, float, aligned_lowp>		aligned_lowp_vec1;

	
	typedef vec<1, double, aligned_highp>	aligned_highp_dvec1;

	
	typedef vec<1, double, aligned_mediump>	aligned_mediump_dvec1;

	
	typedef vec<1, double, aligned_lowp>	aligned_lowp_dvec1;

	
	typedef vec<1, int, aligned_highp>		aligned_highp_ivec1;

	
	typedef vec<1, int, aligned_mediump>	aligned_mediump_ivec1;

	
	typedef vec<1, int, aligned_lowp>		aligned_lowp_ivec1;

	
	typedef vec<1, uint, aligned_highp>		aligned_highp_uvec1;

	
	typedef vec<1, uint, aligned_mediump>	aligned_mediump_uvec1;

	
	typedef vec<1, uint, aligned_lowp>		aligned_lowp_uvec1;

	
	typedef vec<1, bool, aligned_highp>		aligned_highp_bvec1;

	
	typedef vec<1, bool, aligned_mediump>	aligned_mediump_bvec1;

	
	typedef vec<1, bool, aligned_lowp>		aligned_lowp_bvec1;

	
	typedef vec<1, float, packed_highp>		packed_highp_vec1;

	
	typedef vec<1, float, packed_mediump>	packed_mediump_vec1;

	
	typedef vec<1, float, packed_lowp>		packed_lowp_vec1;

	
	typedef vec<1, double, packed_highp>	packed_highp_dvec1;

	
	typedef vec<1, double, packed_mediump>	packed_mediump_dvec1;

	
	typedef vec<1, double, packed_lowp>		packed_lowp_dvec1;

	
	typedef vec<1, int, packed_highp>		packed_highp_ivec1;

	
	typedef vec<1, int, packed_mediump>		packed_mediump_ivec1;

	
	typedef vec<1, int, packed_lowp>		packed_lowp_ivec1;

	
	typedef vec<1, uint, packed_highp>		packed_highp_uvec1;

	
	typedef vec<1, uint, packed_mediump>	packed_mediump_uvec1;

	
	typedef vec<1, uint, packed_lowp>		packed_lowp_uvec1;

	
	typedef vec<1, bool, packed_highp>		packed_highp_bvec1;

	
	typedef vec<1, bool, packed_mediump>	packed_mediump_bvec1;

	
	typedef vec<1, bool, packed_lowp>		packed_lowp_bvec1;

	

	
	typedef vec<2, float, aligned_highp>	aligned_highp_vec2;

	
	typedef vec<2, float, aligned_mediump>	aligned_mediump_vec2;

	
	typedef vec<2, float, aligned_lowp>		aligned_lowp_vec2;

	
	typedef vec<2, double, aligned_highp>	aligned_highp_dvec2;

	
	typedef vec<2, double, aligned_mediump>	aligned_mediump_dvec2;

	
	typedef vec<2, double, aligned_lowp>	aligned_lowp_dvec2;

	
	typedef vec<2, int, aligned_highp>		aligned_highp_ivec2;

	
	typedef vec<2, int, aligned_mediump>	aligned_mediump_ivec2;

	
	typedef vec<2, int, aligned_lowp>		aligned_lowp_ivec2;

	
	typedef vec<2, uint, aligned_highp>		aligned_highp_uvec2;

	
	typedef vec<2, uint, aligned_mediump>	aligned_mediump_uvec2;

	
	typedef vec<2, uint, aligned_lowp>		aligned_lowp_uvec2;

	
	typedef vec<2, bool, aligned_highp>		aligned_highp_bvec2;

	
	typedef vec<2, bool, aligned_mediump>	aligned_mediump_bvec2;

	
	typedef vec<2, bool, aligned_lowp>		aligned_lowp_bvec2;

	
	typedef vec<2, float, packed_highp>		packed_highp_vec2;

	
	typedef vec<2, float, packed_mediump>	packed_mediump_vec2;

	
	typedef vec<2, float, packed_lowp>		packed_lowp_vec2;

	
	typedef vec<2, double, packed_highp>	packed_highp_dvec2;

	
	typedef vec<2, double, packed_mediump>	packed_mediump_dvec2;

	
	typedef vec<2, double, packed_lowp>		packed_lowp_dvec2;

	
	typedef vec<2, int, packed_highp>		packed_highp_ivec2;

	
	typedef vec<2, int, packed_mediump>		packed_mediump_ivec2;

	
	typedef vec<2, int, packed_lowp>		packed_lowp_ivec2;

	
	typedef vec<2, uint, packed_highp>		packed_highp_uvec2;

	
	typedef vec<2, uint, packed_mediump>	packed_mediump_uvec2;

	
	typedef vec<2, uint, packed_lowp>		packed_lowp_uvec2;

	
	typedef vec<2, bool, packed_highp>		packed_highp_bvec2;

	
	typedef vec<2, bool, packed_mediump>	packed_mediump_bvec2;

	
	typedef vec<2, bool, packed_lowp>		packed_lowp_bvec2;

	

	
	typedef vec<3, float, aligned_highp>	aligned_highp_vec3;

	
	typedef vec<3, float, aligned_mediump>	aligned_mediump_vec3;

	
	typedef vec<3, float, aligned_lowp>		aligned_lowp_vec3;

	
	typedef vec<3, double, aligned_highp>	aligned_highp_dvec3;

	
	typedef vec<3, double, aligned_mediump>	aligned_mediump_dvec3;

	
	typedef vec<3, double, aligned_lowp>	aligned_lowp_dvec3;

	
	typedef vec<3, int, aligned_highp>		aligned_highp_ivec3;

	
	typedef vec<3, int, aligned_mediump>	aligned_mediump_ivec3;

	
	typedef vec<3, int, aligned_lowp>		aligned_lowp_ivec3;

	
	typedef vec<3, uint, aligned_highp>		aligned_highp_uvec3;

	
	typedef vec<3, uint, aligned_mediump>	aligned_mediump_uvec3;

	
	typedef vec<3, uint, aligned_lowp>		aligned_lowp_uvec3;

	
	typedef vec<3, bool, aligned_highp>		aligned_highp_bvec3;

	
	typedef vec<3, bool, aligned_mediump>	aligned_mediump_bvec3;

	
	typedef vec<3, bool, aligned_lowp>		aligned_lowp_bvec3;

	
	typedef vec<3, float, packed_highp>		packed_highp_vec3;

	
	typedef vec<3, float, packed_mediump>	packed_mediump_vec3;

	
	typedef vec<3, float, packed_lowp>		packed_lowp_vec3;

	
	typedef vec<3, double, packed_highp>	packed_highp_dvec3;

	
	typedef vec<3, double, packed_mediump>	packed_mediump_dvec3;

	
	typedef vec<3, double, packed_lowp>		packed_lowp_dvec3;

	
	typedef vec<3, int, packed_highp>		packed_highp_ivec3;

	
	typedef vec<3, int, packed_mediump>		packed_mediump_ivec3;

	
	typedef vec<3, int, packed_lowp>		packed_lowp_ivec3;

	
	typedef vec<3, uint, packed_highp>		packed_highp_uvec3;

	
	typedef vec<3, uint, packed_mediump>	packed_mediump_uvec3;

	
	typedef vec<3, uint, packed_lowp>		packed_lowp_uvec3;

	
	typedef vec<3, bool, packed_highp>		packed_highp_bvec3;

	
	typedef vec<3, bool, packed_mediump>	packed_mediump_bvec3;

	
	typedef vec<3, bool, packed_lowp>		packed_lowp_bvec3;

	

	
	typedef vec<4, float, aligned_highp>	aligned_highp_vec4;

	
	typedef vec<4, float, aligned_mediump>	aligned_mediump_vec4;

	
	typedef vec<4, float, aligned_lowp>		aligned_lowp_vec4;

	
	typedef vec<4, double, aligned_highp>	aligned_highp_dvec4;

	
	typedef vec<4, double, aligned_mediump>	aligned_mediump_dvec4;

	
	typedef vec<4, double, aligned_lowp>	aligned_lowp_dvec4;

	
	typedef vec<4, int, aligned_highp>		aligned_highp_ivec4;

	
	typedef vec<4, int, aligned_mediump>	aligned_mediump_ivec4;

	
	typedef vec<4, int, aligned_lowp>		aligned_lowp_ivec4;

	
	typedef vec<4, uint, aligned_highp>		aligned_highp_uvec4;

	
	typedef vec<4, uint, aligned_mediump>	aligned_mediump_uvec4;

	
	typedef vec<4, uint, aligned_lowp>		aligned_lowp_uvec4;

	
	typedef vec<4, bool, aligned_highp>		aligned_highp_bvec4;

	
	typedef vec<4, bool, aligned_mediump>	aligned_mediump_bvec4;

	
	typedef vec<4, bool, aligned_lowp>		aligned_lowp_bvec4;

	
	typedef vec<4, float, packed_highp>		packed_highp_vec4;

	
	typedef vec<4, float, packed_mediump>	packed_mediump_vec4;

	
	typedef vec<4, float, packed_lowp>		packed_lowp_vec4;

	
	typedef vec<4, double, packed_highp>	packed_highp_dvec4;

	
	typedef vec<4, double, packed_mediump>	packed_mediump_dvec4;

	
	typedef vec<4, double, packed_lowp>		packed_lowp_dvec4;

	
	typedef vec<4, int, packed_highp>		packed_highp_ivec4;

	
	typedef vec<4, int, packed_mediump>		packed_mediump_ivec4;

	
	typedef vec<4, int, packed_lowp>		packed_lowp_ivec4;

	
	typedef vec<4, uint, packed_highp>		packed_highp_uvec4;

	
	typedef vec<4, uint, packed_mediump>	packed_mediump_uvec4;

	
	typedef vec<4, uint, packed_lowp>		packed_lowp_uvec4;

	
	typedef vec<4, bool, packed_highp>		packed_highp_bvec4;

	
	typedef vec<4, bool, packed_mediump>	packed_mediump_bvec4;

	
	typedef vec<4, bool, packed_lowp>		packed_lowp_bvec4;

	

	
	typedef mat<2, 2, float, aligned_highp>		aligned_highp_mat2;

	
	typedef mat<2, 2, float, aligned_mediump>	aligned_mediump_mat2;

	
	typedef mat<2, 2, float, aligned_lowp>		aligned_lowp_mat2;

	
	typedef mat<2, 2, double, aligned_highp>	aligned_highp_dmat2;

	
	typedef mat<2, 2, double, aligned_mediump>	aligned_mediump_dmat2;

	
	typedef mat<2, 2, double, aligned_lowp>		aligned_lowp_dmat2;

	
	typedef mat<2, 2, float, packed_highp>		packed_highp_mat2;

	
	typedef mat<2, 2, float, packed_mediump>	packed_mediump_mat2;

	
	typedef mat<2, 2, float, packed_lowp>		packed_lowp_mat2;

	
	typedef mat<2, 2, double, packed_highp>		packed_highp_dmat2;

	
	typedef mat<2, 2, double, packed_mediump>	packed_mediump_dmat2;

	
	typedef mat<2, 2, double, packed_lowp>		packed_lowp_dmat2;

	

	
	typedef mat<3, 3, float, aligned_highp>		aligned_highp_mat3;

	
	typedef mat<3, 3, float, aligned_mediump>	aligned_mediump_mat3;

	
	typedef mat<3, 3, float, aligned_lowp>		aligned_lowp_mat3;

	
	typedef mat<3, 3, double, aligned_highp>	aligned_highp_dmat3;

	
	typedef mat<3, 3, double, aligned_mediump>	aligned_mediump_dmat3;

	
	typedef mat<3, 3, double, aligned_lowp>		aligned_lowp_dmat3;

	
	typedef mat<3, 3, float, packed_highp>		packed_highp_mat3;

	
	typedef mat<3, 3, float, packed_mediump>	packed_mediump_mat3;

	
	typedef mat<3, 3, float, packed_lowp>		packed_lowp_mat3;

	
	typedef mat<3, 3, double, packed_highp>		packed_highp_dmat3;

	
	typedef mat<3, 3, double, packed_mediump>	packed_mediump_dmat3;

	
	typedef mat<3, 3, double, packed_lowp>		packed_lowp_dmat3;

	

	
	typedef mat<4, 4, float, aligned_highp>		aligned_highp_mat4;

	
	typedef mat<4, 4, float, aligned_mediump>	aligned_mediump_mat4;

	
	typedef mat<4, 4, float, aligned_lowp>		aligned_lowp_mat4;

	
	typedef mat<4, 4, double, aligned_highp>	aligned_highp_dmat4;

	
	typedef mat<4, 4, double, aligned_mediump>	aligned_mediump_dmat4;

	
	typedef mat<4, 4, double, aligned_lowp>		aligned_lowp_dmat4;

	
	typedef mat<4, 4, float, packed_highp>		packed_highp_mat4;

	
	typedef mat<4, 4, float, packed_mediump>	packed_mediump_mat4;

	
	typedef mat<4, 4, float, packed_lowp>		packed_lowp_mat4;

	
	typedef mat<4, 4, double, packed_highp>		packed_highp_dmat4;

	
	typedef mat<4, 4, double, packed_mediump>	packed_mediump_dmat4;

	
	typedef mat<4, 4, double, packed_lowp>		packed_lowp_dmat4;

	

	
	typedef mat<2, 2, float, aligned_highp>		aligned_highp_mat2x2;

	
	typedef mat<2, 2, float, aligned_mediump>	aligned_mediump_mat2x2;

	
	typedef mat<2, 2, float, aligned_lowp>		aligned_lowp_mat2x2;

	
	typedef mat<2, 2, double, aligned_highp>	aligned_highp_dmat2x2;

	
	typedef mat<2, 2, double, aligned_mediump>	aligned_mediump_dmat2x2;

	
	typedef mat<2, 2, double, aligned_lowp>		aligned_lowp_dmat2x2;

	
	typedef mat<2, 2, float, packed_highp>		packed_highp_mat2x2;

	
	typedef mat<2, 2, float, packed_mediump>	packed_mediump_mat2x2;

	
	typedef mat<2, 2, float, packed_lowp>		packed_lowp_mat2x2;

	
	typedef mat<2, 2, double, packed_highp>		packed_highp_dmat2x2;

	
	typedef mat<2, 2, double, packed_mediump>	packed_mediump_dmat2x2;

	
	typedef mat<2, 2, double, packed_lowp>		packed_lowp_dmat2x2;

	

	
	typedef mat<2, 3, float, aligned_highp>		aligned_highp_mat2x3;

	
	typedef mat<2, 3, float, aligned_mediump>	aligned_mediump_mat2x3;

	
	typedef mat<2, 3, float, aligned_lowp>		aligned_lowp_mat2x3;

	
	typedef mat<2, 3, double, aligned_highp>	aligned_highp_dmat2x3;

	
	typedef mat<2, 3, double, aligned_mediump>	aligned_mediump_dmat2x3;

	
	typedef mat<2, 3, double, aligned_lowp>		aligned_lowp_dmat2x3;

	
	typedef mat<2, 3, float, packed_highp>		packed_highp_mat2x3;

	
	typedef mat<2, 3, float, packed_mediump>	packed_mediump_mat2x3;

	
	typedef mat<2, 3, float, packed_lowp>		packed_lowp_mat2x3;

	
	typedef mat<2, 3, double, packed_highp>		packed_highp_dmat2x3;

	
	typedef mat<2, 3, double, packed_mediump>	packed_mediump_dmat2x3;

	
	typedef mat<2, 3, double, packed_lowp>		packed_lowp_dmat2x3;

	

	
	typedef mat<2, 4, float, aligned_highp>		aligned_highp_mat2x4;

	
	typedef mat<2, 4, float, aligned_mediump>	aligned_mediump_mat2x4;

	
	typedef mat<2, 4, float, aligned_lowp>		aligned_lowp_mat2x4;

	
	typedef mat<2, 4, double, aligned_highp>	aligned_highp_dmat2x4;

	
	typedef mat<2, 4, double, aligned_mediump>	aligned_mediump_dmat2x4;

	
	typedef mat<2, 4, double, aligned_lowp>		aligned_lowp_dmat2x4;

	
	typedef mat<2, 4, float, packed_highp>		packed_highp_mat2x4;

	
	typedef mat<2, 4, float, packed_mediump>	packed_mediump_mat2x4;

	
	typedef mat<2, 4, float, packed_lowp>		packed_lowp_mat2x4;

	
	typedef mat<2, 4, double, packed_highp>		packed_highp_dmat2x4;

	
	typedef mat<2, 4, double, packed_mediump>	packed_mediump_dmat2x4;

	
	typedef mat<2, 4, double, packed_lowp>		packed_lowp_dmat2x4;

	

	
	typedef mat<3, 2, float, aligned_highp>		aligned_highp_mat3x2;

	
	typedef mat<3, 2, float, aligned_mediump>	aligned_mediump_mat3x2;

	
	typedef mat<3, 2, float, aligned_lowp>		aligned_lowp_mat3x2;

	
	typedef mat<3, 2, double, aligned_highp>	aligned_highp_dmat3x2;

	
	typedef mat<3, 2, double, aligned_mediump>	aligned_mediump_dmat3x2;

	
	typedef mat<3, 2, double, aligned_lowp>		aligned_lowp_dmat3x2;

	
	typedef mat<3, 2, float, packed_highp>		packed_highp_mat3x2;

	
	typedef mat<3, 2, float, packed_mediump>	packed_mediump_mat3x2;

	
	typedef mat<3, 2, float, packed_lowp>		packed_lowp_mat3x2;

	
	typedef mat<3, 2, double, packed_highp>		packed_highp_dmat3x2;

	
	typedef mat<3, 2, double, packed_mediump>	packed_mediump_dmat3x2;

	
	typedef mat<3, 2, double, packed_lowp>		packed_lowp_dmat3x2;

	

	
	typedef mat<3, 3, float, aligned_highp>		aligned_highp_mat3x3;

	
	typedef mat<3, 3, float, aligned_mediump>	aligned_mediump_mat3x3;

	
	typedef mat<3, 3, float, aligned_lowp>		aligned_lowp_mat3x3;

	
	typedef mat<3, 3, double, aligned_highp>	aligned_highp_dmat3x3;

	
	typedef mat<3, 3, double, aligned_mediump>	aligned_mediump_dmat3x3;

	
	typedef mat<3, 3, double, aligned_lowp>		aligned_lowp_dmat3x3;

	
	typedef mat<3, 3, float, packed_highp>		packed_highp_mat3x3;

	
	typedef mat<3, 3, float, packed_mediump>	packed_mediump_mat3x3;

	
	typedef mat<3, 3, float, packed_lowp>		packed_lowp_mat3x3;

	
	typedef mat<3, 3, double, packed_highp>		packed_highp_dmat3x3;

	
	typedef mat<3, 3, double, packed_mediump>	packed_mediump_dmat3x3;

	
	typedef mat<3, 3, double, packed_lowp>		packed_lowp_dmat3x3;

	

	
	typedef mat<3, 4, float, aligned_highp>		aligned_highp_mat3x4;

	
	typedef mat<3, 4, float, aligned_mediump>	aligned_mediump_mat3x4;

	
	typedef mat<3, 4, float, aligned_lowp>		aligned_lowp_mat3x4;

	
	typedef mat<3, 4, double, aligned_highp>	aligned_highp_dmat3x4;

	
	typedef mat<3, 4, double, aligned_mediump>	aligned_mediump_dmat3x4;

	
	typedef mat<3, 4, double, aligned_lowp>		aligned_lowp_dmat3x4;

	
	typedef mat<3, 4, float, packed_highp>		packed_highp_mat3x4;

	
	typedef mat<3, 4, float, packed_mediump>	packed_mediump_mat3x4;

	
	typedef mat<3, 4, float, packed_lowp>		packed_lowp_mat3x4;

	
	typedef mat<3, 4, double, packed_highp>		packed_highp_dmat3x4;

	
	typedef mat<3, 4, double, packed_mediump>	packed_mediump_dmat3x4;

	
	typedef mat<3, 4, double, packed_lowp>		packed_lowp_dmat3x4;

	

	
	typedef mat<4, 2, float, aligned_highp>		aligned_highp_mat4x2;

	
	typedef mat<4, 2, float, aligned_mediump>	aligned_mediump_mat4x2;

	
	typedef mat<4, 2, float, aligned_lowp>		aligned_lowp_mat4x2;

	
	typedef mat<4, 2, double, aligned_highp>	aligned_highp_dmat4x2;

	
	typedef mat<4, 2, double, aligned_mediump>	aligned_mediump_dmat4x2;

	
	typedef mat<4, 2, double, aligned_lowp>		aligned_lowp_dmat4x2;

	
	typedef mat<4, 2, float, packed_highp>		packed_highp_mat4x2;

	
	typedef mat<4, 2, float, packed_mediump>	packed_mediump_mat4x2;

	
	typedef mat<4, 2, float, packed_lowp>		packed_lowp_mat4x2;

	
	typedef mat<4, 2, double, packed_highp>		packed_highp_dmat4x2;

	
	typedef mat<4, 2, double, packed_mediump>	packed_mediump_dmat4x2;

	
	typedef mat<4, 2, double, packed_lowp>		packed_lowp_dmat4x2;

	

	
	typedef mat<4, 3, float, aligned_highp>		aligned_highp_mat4x3;

	
	typedef mat<4, 3, float, aligned_mediump>	aligned_mediump_mat4x3;

	
	typedef mat<4, 3, float, aligned_lowp>		aligned_lowp_mat4x3;

	
	typedef mat<4, 3, double, aligned_highp>	aligned_highp_dmat4x3;

	
	typedef mat<4, 3, double, aligned_mediump>	aligned_mediump_dmat4x3;

	
	typedef mat<4, 3, double, aligned_lowp>		aligned_lowp_dmat4x3;

	
	typedef mat<4, 3, float, packed_highp>		packed_highp_mat4x3;

	
	typedef mat<4, 3, float, packed_mediump>	packed_mediump_mat4x3;

	
	typedef mat<4, 3, float, packed_lowp>		packed_lowp_mat4x3;

	
	typedef mat<4, 3, double, packed_highp>		packed_highp_dmat4x3;

	
	typedef mat<4, 3, double, packed_mediump>	packed_mediump_dmat4x3;

	
	typedef mat<4, 3, double, packed_lowp>		packed_lowp_dmat4x3;

	

	
	typedef mat<4, 4, float, aligned_highp>		aligned_highp_mat4x4;

	
	typedef mat<4, 4, float, aligned_mediump>	aligned_mediump_mat4x4;

	
	typedef mat<4, 4, float, aligned_lowp>		aligned_lowp_mat4x4;

	
	typedef mat<4, 4, double, aligned_highp>	aligned_highp_dmat4x4;

	
	typedef mat<4, 4, double, aligned_mediump>	aligned_mediump_dmat4x4;

	
	typedef mat<4, 4, double, aligned_lowp>		aligned_lowp_dmat4x4;

	
	typedef mat<4, 4, float, packed_highp>		packed_highp_mat4x4;

	
	typedef mat<4, 4, float, packed_mediump>	packed_mediump_mat4x4;

	
	typedef mat<4, 4, float, packed_lowp>		packed_lowp_mat4x4;

	
	typedef mat<4, 4, double, packed_highp>		packed_highp_dmat4x4;

	
	typedef mat<4, 4, double, packed_mediump>	packed_mediump_dmat4x4;

	
	typedef mat<4, 4, double, packed_lowp>		packed_lowp_dmat4x4;

	

#if(defined(GLM_PRECISION_LOWP_FLOAT))
	typedef aligned_lowp_vec1			aligned_vec1;
	typedef aligned_lowp_vec2			aligned_vec2;
	typedef aligned_lowp_vec3			aligned_vec3;
	typedef aligned_lowp_vec4			aligned_vec4;
	typedef packed_lowp_vec1			packed_vec1;
	typedef packed_lowp_vec2			packed_vec2;
	typedef packed_lowp_vec3			packed_vec3;
	typedef packed_lowp_vec4			packed_vec4;

	typedef aligned_lowp_mat2			aligned_mat2;
	typedef aligned_lowp_mat3			aligned_mat3;
	typedef aligned_lowp_mat4			aligned_mat4;
	typedef packed_lowp_mat2			packed_mat2;
	typedef packed_lowp_mat3			packed_mat3;
	typedef packed_lowp_mat4			packed_mat4;

	typedef aligned_lowp_mat2x2			aligned_mat2x2;
	typedef aligned_lowp_mat2x3			aligned_mat2x3;
	typedef aligned_lowp_mat2x4			aligned_mat2x4;
	typedef aligned_lowp_mat3x2			aligned_mat3x2;
	typedef aligned_lowp_mat3x3			aligned_mat3x3;
	typedef aligned_lowp_mat3x4			aligned_mat3x4;
	typedef aligned_lowp_mat4x2			aligned_mat4x2;
	typedef aligned_lowp_mat4x3			aligned_mat4x3;
	typedef aligned_lowp_mat4x4			aligned_mat4x4;
	typedef packed_lowp_mat2x2			packed_mat2x2;
	typedef packed_lowp_mat2x3			packed_mat2x3;
	typedef packed_lowp_mat2x4			packed_mat2x4;
	typedef packed_lowp_mat3x2			packed_mat3x2;
	typedef packed_lowp_mat3x3			packed_mat3x3;
	typedef packed_lowp_mat3x4			packed_mat3x4;
	typedef packed_lowp_mat4x2			packed_mat4x2;
	typedef packed_lowp_mat4x3			packed_mat4x3;
	typedef packed_lowp_mat4x4			packed_mat4x4;
#elif(defined(GLM_PRECISION_MEDIUMP_FLOAT))
	typedef aligned_mediump_vec1		aligned_vec1;
	typedef aligned_mediump_vec2		aligned_vec2;
	typedef aligned_mediump_vec3		aligned_vec3;
	typedef aligned_mediump_vec4		aligned_vec4;
	typedef packed_mediump_vec1			packed_vec1;
	typedef packed_mediump_vec2			packed_vec2;
	typedef packed_mediump_vec3			packed_vec3;
	typedef packed_mediump_vec4			packed_vec4;

	typedef aligned_mediump_mat2		aligned_mat2;
	typedef aligned_mediump_mat3		aligned_mat3;
	typedef aligned_mediump_mat4		aligned_mat4;
	typedef packed_mediump_mat2			packed_mat2;
	typedef packed_mediump_mat3			packed_mat3;
	typedef packed_mediump_mat4			packed_mat4;

	typedef aligned_mediump_mat2x2		aligned_mat2x2;
	typedef aligned_mediump_mat2x3		aligned_mat2x3;
	typedef aligned_mediump_mat2x4		aligned_mat2x4;
	typedef aligned_mediump_mat3x2		aligned_mat3x2;
	typedef aligned_mediump_mat3x3		aligned_mat3x3;
	typedef aligned_mediump_mat3x4		aligned_mat3x4;
	typedef aligned_mediump_mat4x2		aligned_mat4x2;
	typedef aligned_mediump_mat4x3		aligned_mat4x3;
	typedef aligned_mediump_mat4x4		aligned_mat4x4;
	typedef packed_mediump_mat2x2		packed_mat2x2;
	typedef packed_mediump_mat2x3		packed_mat2x3;
	typedef packed_mediump_mat2x4		packed_mat2x4;
	typedef packed_mediump_mat3x2		packed_mat3x2;
	typedef packed_mediump_mat3x3		packed_mat3x3;
	typedef packed_mediump_mat3x4		packed_mat3x4;
	typedef packed_mediump_mat4x2		packed_mat4x2;
	typedef packed_mediump_mat4x3		packed_mat4x3;
	typedef packed_mediump_mat4x4		packed_mat4x4;
#else 
	
	typedef aligned_highp_vec1			aligned_vec1;

	
	typedef aligned_highp_vec2			aligned_vec2;

	
	typedef aligned_highp_vec3			aligned_vec3;

	
	typedef aligned_highp_vec4 			aligned_vec4;

	
	typedef packed_highp_vec1			packed_vec1;

	
	typedef packed_highp_vec2			packed_vec2;

	
	typedef packed_highp_vec3			packed_vec3;

	
	typedef packed_highp_vec4			packed_vec4;

	
	typedef aligned_highp_mat2			aligned_mat2;

	
	typedef aligned_highp_mat3			aligned_mat3;

	
	typedef aligned_highp_mat4			aligned_mat4;

	
	typedef packed_highp_mat2			packed_mat2;

	
	typedef packed_highp_mat3			packed_mat3;

	
	typedef packed_highp_mat4			packed_mat4;

	
	typedef aligned_highp_mat2x2		aligned_mat2x2;

	
	typedef aligned_highp_mat2x3		aligned_mat2x3;

	
	typedef aligned_highp_mat2x4		aligned_mat2x4;

	
	typedef aligned_highp_mat3x2		aligned_mat3x2;

	
	typedef aligned_highp_mat3x3		aligned_mat3x3;

	
	typedef aligned_highp_mat3x4		aligned_mat3x4;

	
	typedef aligned_highp_mat4x2		aligned_mat4x2;

	
	typedef aligned_highp_mat4x3		aligned_mat4x3;

	
	typedef aligned_highp_mat4x4		aligned_mat4x4;

	
	typedef packed_highp_mat2x2			packed_mat2x2;

	
	typedef packed_highp_mat2x3			packed_mat2x3;

	
	typedef packed_highp_mat2x4			packed_mat2x4;

	
	typedef packed_highp_mat3x2			packed_mat3x2;

	
	typedef packed_highp_mat3x3			packed_mat3x3;

	
	typedef packed_highp_mat3x4			packed_mat3x4;

	
	typedef packed_highp_mat4x2			packed_mat4x2;

	
	typedef packed_highp_mat4x3			packed_mat4x3;

	
	typedef packed_highp_mat4x4			packed_mat4x4;
#endif

#if(defined(GLM_PRECISION_LOWP_DOUBLE))
	typedef aligned_lowp_dvec1			aligned_dvec1;
	typedef aligned_lowp_dvec2			aligned_dvec2;
	typedef aligned_lowp_dvec3			aligned_dvec3;
	typedef aligned_lowp_dvec4			aligned_dvec4;
	typedef packed_lowp_dvec1			packed_dvec1;
	typedef packed_lowp_dvec2			packed_dvec2;
	typedef packed_lowp_dvec3			packed_dvec3;
	typedef packed_lowp_dvec4			packed_dvec4;

	typedef aligned_lowp_dmat2			aligned_dmat2;
	typedef aligned_lowp_dmat3			aligned_dmat3;
	typedef aligned_lowp_dmat4			aligned_dmat4;
	typedef packed_lowp_dmat2			packed_dmat2;
	typedef packed_lowp_dmat3			packed_dmat3;
	typedef packed_lowp_dmat4			packed_dmat4;

	typedef aligned_lowp_dmat2x2		aligned_dmat2x2;
	typedef aligned_lowp_dmat2x3		aligned_dmat2x3;
	typedef aligned_lowp_dmat2x4		aligned_dmat2x4;
	typedef aligned_lowp_dmat3x2		aligned_dmat3x2;
	typedef aligned_lowp_dmat3x3		aligned_dmat3x3;
	typedef aligned_lowp_dmat3x4		aligned_dmat3x4;
	typedef aligned_lowp_dmat4x2		aligned_dmat4x2;
	typedef aligned_lowp_dmat4x3		aligned_dmat4x3;
	typedef aligned_lowp_dmat4x4		aligned_dmat4x4;
	typedef packed_lowp_dmat2x2			packed_dmat2x2;
	typedef packed_lowp_dmat2x3			packed_dmat2x3;
	typedef packed_lowp_dmat2x4			packed_dmat2x4;
	typedef packed_lowp_dmat3x2			packed_dmat3x2;
	typedef packed_lowp_dmat3x3			packed_dmat3x3;
	typedef packed_lowp_dmat3x4			packed_dmat3x4;
	typedef packed_lowp_dmat4x2			packed_dmat4x2;
	typedef packed_lowp_dmat4x3			packed_dmat4x3;
	typedef packed_lowp_dmat4x4			packed_dmat4x4;
#elif(defined(GLM_PRECISION_MEDIUMP_DOUBLE))
	typedef aligned_mediump_dvec1		aligned_dvec1;
	typedef aligned_mediump_dvec2		aligned_dvec2;
	typedef aligned_mediump_dvec3		aligned_dvec3;
	typedef aligned_mediump_dvec4		aligned_dvec4;
	typedef packed_mediump_dvec1		packed_dvec1;
	typedef packed_mediump_dvec2		packed_dvec2;
	typedef packed_mediump_dvec3		packed_dvec3;
	typedef packed_mediump_dvec4		packed_dvec4;

	typedef aligned_mediump_dmat2		aligned_dmat2;
	typedef aligned_mediump_dmat3		aligned_dmat3;
	typedef aligned_mediump_dmat4		aligned_dmat4;
	typedef packed_mediump_dmat2		packed_dmat2;
	typedef packed_mediump_dmat3		packed_dmat3;
	typedef packed_mediump_dmat4		packed_dmat4;

	typedef aligned_mediump_dmat2x2		aligned_dmat2x2;
	typedef aligned_mediump_dmat2x3		aligned_dmat2x3;
	typedef aligned_mediump_dmat2x4		aligned_dmat2x4;
	typedef aligned_mediump_dmat3x2		aligned_dmat3x2;
	typedef aligned_mediump_dmat3x3		aligned_dmat3x3;
	typedef aligned_mediump_dmat3x4		aligned_dmat3x4;
	typedef aligned_mediump_dmat4x2		aligned_dmat4x2;
	typedef aligned_mediump_dmat4x3		aligned_dmat4x3;
	typedef aligned_mediump_dmat4x4		aligned_dmat4x4;
	typedef packed_mediump_dmat2x2		packed_dmat2x2;
	typedef packed_mediump_dmat2x3		packed_dmat2x3;
	typedef packed_mediump_dmat2x4		packed_dmat2x4;
	typedef packed_mediump_dmat3x2		packed_dmat3x2;
	typedef packed_mediump_dmat3x3		packed_dmat3x3;
	typedef packed_mediump_dmat3x4		packed_dmat3x4;
	typedef packed_mediump_dmat4x2		packed_dmat4x2;
	typedef packed_mediump_dmat4x3		packed_dmat4x3;
	typedef packed_mediump_dmat4x4		packed_dmat4x4;
#else 
	
	typedef aligned_highp_dvec1			aligned_dvec1;

	
	typedef aligned_highp_dvec2			aligned_dvec2;

	
	typedef aligned_highp_dvec3			aligned_dvec3;

	
	typedef aligned_highp_dvec4			aligned_dvec4;

	
	typedef packed_highp_dvec1			packed_dvec1;

	
	typedef packed_highp_dvec2			packed_dvec2;

	
	typedef packed_highp_dvec3			packed_dvec3;

	
	typedef packed_highp_dvec4			packed_dvec4;

	
	typedef aligned_highp_dmat2			aligned_dmat2;

	
	typedef aligned_highp_dmat3			aligned_dmat3;

	
	typedef aligned_highp_dmat4			aligned_dmat4;

	
	typedef packed_highp_dmat2			packed_dmat2;

	
	typedef packed_highp_dmat3			packed_dmat3;

	
	typedef packed_highp_dmat4			packed_dmat4;

	
	typedef aligned_highp_dmat2x2		aligned_dmat2x2;

	
	typedef aligned_highp_dmat2x3		aligned_dmat2x3;

	
	typedef aligned_highp_dmat2x4		aligned_dmat2x4;

	
	typedef aligned_highp_dmat3x2		aligned_dmat3x2;

	
	typedef aligned_highp_dmat3x3		aligned_dmat3x3;

	
	typedef aligned_highp_dmat3x4		aligned_dmat3x4;

	
	typedef aligned_highp_dmat4x2		aligned_dmat4x2;

	
	typedef aligned_highp_dmat4x3		aligned_dmat4x3;

	
	typedef aligned_highp_dmat4x4		aligned_dmat4x4;

	
	typedef packed_highp_dmat2x2		packed_dmat2x2;

	
	typedef packed_highp_dmat2x3		packed_dmat2x3;

	
	typedef packed_highp_dmat2x4		packed_dmat2x4;

	
	typedef packed_highp_dmat3x2		packed_dmat3x2;

	
	typedef packed_highp_dmat3x3		packed_dmat3x3;

	
	typedef packed_highp_dmat3x4		packed_dmat3x4;

	
	typedef packed_highp_dmat4x2		packed_dmat4x2;

	
	typedef packed_highp_dmat4x3		packed_dmat4x3;

	
	typedef packed_highp_dmat4x4		packed_dmat4x4;
#endif

#if(defined(GLM_PRECISION_LOWP_INT))
	typedef aligned_lowp_ivec1			aligned_ivec1;
	typedef aligned_lowp_ivec2			aligned_ivec2;
	typedef aligned_lowp_ivec3			aligned_ivec3;
	typedef aligned_lowp_ivec4			aligned_ivec4;
#elif(defined(GLM_PRECISION_MEDIUMP_INT))
	typedef aligned_mediump_ivec1		aligned_ivec1;
	typedef aligned_mediump_ivec2		aligned_ivec2;
	typedef aligned_mediump_ivec3		aligned_ivec3;
	typedef aligned_mediump_ivec4		aligned_ivec4;
#else 
	
	typedef aligned_highp_ivec1			aligned_ivec1;

	
	typedef aligned_highp_ivec2			aligned_ivec2;

	
	typedef aligned_highp_ivec3			aligned_ivec3;

	
	typedef aligned_highp_ivec4			aligned_ivec4;

	
	typedef packed_highp_ivec1			packed_ivec1;

	
	typedef packed_highp_ivec2			packed_ivec2;

	
	typedef packed_highp_ivec3			packed_ivec3;

	
	typedef packed_highp_ivec4			packed_ivec4;
#endif

	

#if(defined(GLM_PRECISION_LOWP_UINT))
	typedef aligned_lowp_uvec1			aligned_uvec1;
	typedef aligned_lowp_uvec2			aligned_uvec2;
	typedef aligned_lowp_uvec3			aligned_uvec3;
	typedef aligned_lowp_uvec4			aligned_uvec4;
#elif(defined(GLM_PRECISION_MEDIUMP_UINT))
	typedef aligned_mediump_uvec1		aligned_uvec1;
	typedef aligned_mediump_uvec2		aligned_uvec2;
	typedef aligned_mediump_uvec3		aligned_uvec3;
	typedef aligned_mediump_uvec4		aligned_uvec4;
#else 
	
	typedef aligned_highp_uvec1			aligned_uvec1;

	
	typedef aligned_highp_uvec2			aligned_uvec2;

	
	typedef aligned_highp_uvec3			aligned_uvec3;

	
	typedef aligned_highp_uvec4			aligned_uvec4;

	
	typedef packed_highp_uvec1			packed_uvec1;

	
	typedef packed_highp_uvec2			packed_uvec2;

	
	typedef packed_highp_uvec3			packed_uvec3;

	
	typedef packed_highp_uvec4			packed_uvec4;
#endif

#if(defined(GLM_PRECISION_LOWP_BOOL))
	typedef aligned_lowp_bvec1			aligned_bvec1;
	typedef aligned_lowp_bvec2			aligned_bvec2;
	typedef aligned_lowp_bvec3			aligned_bvec3;
	typedef aligned_lowp_bvec4			aligned_bvec4;
#elif(defined(GLM_PRECISION_MEDIUMP_BOOL))
	typedef aligned_mediump_bvec1		aligned_bvec1;
	typedef aligned_mediump_bvec2		aligned_bvec2;
	typedef aligned_mediump_bvec3		aligned_bvec3;
	typedef aligned_mediump_bvec4		aligned_bvec4;
#else 
	
	typedef aligned_highp_bvec1			aligned_bvec1;

	
	typedef aligned_highp_bvec2			aligned_bvec2;

	
	typedef aligned_highp_bvec3			aligned_bvec3;

	
	typedef aligned_highp_bvec4			aligned_bvec4;

	
	typedef packed_highp_bvec1			packed_bvec1;

	
	typedef packed_highp_bvec2			packed_bvec2;

	
	typedef packed_highp_bvec3			packed_bvec3;

	
	typedef packed_highp_bvec4			packed_bvec4;
#endif

	
}
