












#pragma once


#include "../mat3x3.hpp"
#include "../vec2.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_matrix_transform_2d is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_matrix_transform_2d extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> translate(
		mat<3, 3, T, Q> const& m,
		vec<2, T, Q> const& v);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> rotate(
		mat<3, 3, T, Q> const& m,
		T angle);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> scale(
		mat<3, 3, T, Q> const& m,
		vec<2, T, Q> const& v);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> shearX(
		mat<3, 3, T, Q> const& m,
		T y);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> shearY(
		mat<3, 3, T, Q> const& m,
		T x);

	
}

#include "matrix_transform_2d.inl"
