












#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GTX_associated_min_max is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_associated_min_max extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL U associatedMin(T x, U a, T y, U b);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<2, U, Q> associatedMin(
		vec<L, T, Q> const& x, vec<L, U, Q> const& a,
		vec<L, T, Q> const& y, vec<L, U, Q> const& b);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMin(
		T x, const vec<L, U, Q>& a,
		T y, const vec<L, U, Q>& b);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMin(
		vec<L, T, Q> const& x, U a,
		vec<L, T, Q> const& y, U b);

	
	
	template<typename T, typename U>
	GLM_FUNC_DECL U associatedMin(
		T x, U a,
		T y, U b,
		T z, U c);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMin(
		vec<L, T, Q> const& x, vec<L, U, Q> const& a,
		vec<L, T, Q> const& y, vec<L, U, Q> const& b,
		vec<L, T, Q> const& z, vec<L, U, Q> const& c);

	
	
	template<typename T, typename U>
	GLM_FUNC_DECL U associatedMin(
		T x, U a,
		T y, U b,
		T z, U c,
		T w, U d);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMin(
		vec<L, T, Q> const& x, vec<L, U, Q> const& a,
		vec<L, T, Q> const& y, vec<L, U, Q> const& b,
		vec<L, T, Q> const& z, vec<L, U, Q> const& c,
		vec<L, T, Q> const& w, vec<L, U, Q> const& d);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMin(
		T x, vec<L, U, Q> const& a,
		T y, vec<L, U, Q> const& b,
		T z, vec<L, U, Q> const& c,
		T w, vec<L, U, Q> const& d);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMin(
		vec<L, T, Q> const& x, U a,
		vec<L, T, Q> const& y, U b,
		vec<L, T, Q> const& z, U c,
		vec<L, T, Q> const& w, U d);

	
	
	template<typename T, typename U>
	GLM_FUNC_DECL U associatedMax(T x, U a, T y, U b);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<2, U, Q> associatedMax(
		vec<L, T, Q> const& x, vec<L, U, Q> const& a,
		vec<L, T, Q> const& y, vec<L, U, Q> const& b);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> associatedMax(
		T x, vec<L, U, Q> const& a,
		T y, vec<L, U, Q> const& b);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMax(
		vec<L, T, Q> const& x, U a,
		vec<L, T, Q> const& y, U b);

	
	
	template<typename T, typename U>
	GLM_FUNC_DECL U associatedMax(
		T x, U a,
		T y, U b,
		T z, U c);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMax(
		vec<L, T, Q> const& x, vec<L, U, Q> const& a,
		vec<L, T, Q> const& y, vec<L, U, Q> const& b,
		vec<L, T, Q> const& z, vec<L, U, Q> const& c);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> associatedMax(
		T x, vec<L, U, Q> const& a,
		T y, vec<L, U, Q> const& b,
		T z, vec<L, U, Q> const& c);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMax(
		vec<L, T, Q> const& x, U a,
		vec<L, T, Q> const& y, U b,
		vec<L, T, Q> const& z, U c);

	
	
	template<typename T, typename U>
	GLM_FUNC_DECL U associatedMax(
		T x, U a,
		T y, U b,
		T z, U c,
		T w, U d);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMax(
		vec<L, T, Q> const& x, vec<L, U, Q> const& a,
		vec<L, T, Q> const& y, vec<L, U, Q> const& b,
		vec<L, T, Q> const& z, vec<L, U, Q> const& c,
		vec<L, T, Q> const& w, vec<L, U, Q> const& d);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMax(
		T x, vec<L, U, Q> const& a,
		T y, vec<L, U, Q> const& b,
		T z, vec<L, U, Q> const& c,
		T w, vec<L, U, Q> const& d);

	
	
	template<length_t L, typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<L, U, Q> associatedMax(
		vec<L, T, Q> const& x, U a,
		vec<L, T, Q> const& y, U b,
		vec<L, T, Q> const& z, U c,
		vec<L, T, Q> const& w, U d);

	
} 

#include "associated_min_max.inl"
