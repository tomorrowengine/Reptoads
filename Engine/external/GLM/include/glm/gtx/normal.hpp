












#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_normal is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_normal extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> triangleNormal(vec<3, T, Q> const& p1, vec<3, T, Q> const& p2, vec<3, T, Q> const& p3);

	
}

#include "normal.inl"
