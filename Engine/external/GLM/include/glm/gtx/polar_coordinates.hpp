











#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_polar_coordinates is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_polar_coordinates extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> polar(
		vec<3, T, Q> const& euclidean);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> euclidean(
		vec<2, T, Q> const& polar);

	
}

#include "polar_coordinates.inl"
