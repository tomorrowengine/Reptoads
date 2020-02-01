











#pragma once


#include "../glm.hpp"
#include "../gtc/integer.hpp"
#include "../gtx/component_wise.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_texture is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_texture extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	template <length_t L, typename T, qualifier Q>
	T levels(vec<L, T, Q> const& Extent);

	
}

#include "texture.inl"

