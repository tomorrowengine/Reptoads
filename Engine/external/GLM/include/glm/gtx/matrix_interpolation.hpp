












#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_matrix_interpolation is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_matrix_interpolation extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL void axisAngle(
		mat<4, 4, T, Q> const& Mat, vec<3, T, Q> & Axis, T & Angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> axisAngleMatrix(
		vec<3, T, Q> const& Axis, T const Angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> extractMatrixRotation(
		mat<4, 4, T, Q> const& Mat);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> interpolate(
		mat<4, 4, T, Q> const& m1, mat<4, 4, T, Q> const& m2, T const Delta);

	
}

#include "matrix_interpolation.inl"
