













#pragma once


#include "../glm.hpp"
#include "../gtc/epsilon.hpp"
#include "../gtc/quaternion.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_rotate_normalized_axis is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_rotate_normalized_axis extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> rotateNormalizedAxis(
		mat<4, 4, T, Q> const& m,
		T const& angle,
		vec<3, T, Q> const& axis);

	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> rotateNormalizedAxis(
		qua<T, Q> const& q,
		T const& angle,
		vec<3, T, Q> const& axis);

	
}

#include "rotate_normalized_axis.inl"
