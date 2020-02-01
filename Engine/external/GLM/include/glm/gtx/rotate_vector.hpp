












#pragma once


#include "../gtx/transform.hpp"
#include "../gtc/epsilon.hpp"
#include "../ext/vector_relational.hpp"
#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_rotate_vector is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_rotate_vector extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> slerp(
		vec<3, T, Q> const& x,
		vec<3, T, Q> const& y,
		T const& a);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> rotate(
		vec<2, T, Q> const& v,
		T const& angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> rotate(
		vec<3, T, Q> const& v,
		T const& angle,
		vec<3, T, Q> const& normal);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> rotate(
		vec<4, T, Q> const& v,
		T const& angle,
		vec<3, T, Q> const& normal);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> rotateX(
		vec<3, T, Q> const& v,
		T const& angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> rotateY(
		vec<3, T, Q> const& v,
		T const& angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> rotateZ(
		vec<3, T, Q> const& v,
		T const& angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> rotateX(
		vec<4, T, Q> const& v,
		T const& angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> rotateY(
		vec<4, T, Q> const& v,
		T const& angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> rotateZ(
		vec<4, T, Q> const& v,
		T const& angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> orientation(
		vec<3, T, Q> const& Normal,
		vec<3, T, Q> const& Up);

	
}

#include "rotate_vector.inl"
