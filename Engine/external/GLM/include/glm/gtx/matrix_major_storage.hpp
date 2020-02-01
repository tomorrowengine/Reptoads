












#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_matrix_major_storage is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_matrix_major_storage extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<2, 2, T, Q> rowMajor2(
		vec<2, T, Q> const& v1,
		vec<2, T, Q> const& v2);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<2, 2, T, Q> rowMajor2(
		mat<2, 2, T, Q> const& m);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> rowMajor3(
		vec<3, T, Q> const& v1,
		vec<3, T, Q> const& v2,
		vec<3, T, Q> const& v3);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> rowMajor3(
		mat<3, 3, T, Q> const& m);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> rowMajor4(
		vec<4, T, Q> const& v1,
		vec<4, T, Q> const& v2,
		vec<4, T, Q> const& v3,
		vec<4, T, Q> const& v4);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> rowMajor4(
		mat<4, 4, T, Q> const& m);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<2, 2, T, Q> colMajor2(
		vec<2, T, Q> const& v1,
		vec<2, T, Q> const& v2);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<2, 2, T, Q> colMajor2(
		mat<2, 2, T, Q> const& m);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> colMajor3(
		vec<3, T, Q> const& v1,
		vec<3, T, Q> const& v2,
		vec<3, T, Q> const& v3);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> colMajor3(
		mat<3, 3, T, Q> const& m);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> colMajor4(
		vec<4, T, Q> const& v1,
		vec<4, T, Q> const& v2,
		vec<4, T, Q> const& v3,
		vec<4, T, Q> const& v4);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> colMajor4(
		mat<4, 4, T, Q> const& m);

	
}

#include "matrix_major_storage.inl"
