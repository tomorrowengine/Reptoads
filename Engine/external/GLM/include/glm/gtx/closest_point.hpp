











#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_closest_point is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_closest_point extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> closestPointOnLine(
		vec<3, T, Q> const& point,
		vec<3, T, Q> const& a,
		vec<3, T, Q> const& b);

	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> closestPointOnLine(
		vec<2, T, Q> const& point,
		vec<2, T, Q> const& a,
		vec<2, T, Q> const& b);

	
}

#include "closest_point.inl"
