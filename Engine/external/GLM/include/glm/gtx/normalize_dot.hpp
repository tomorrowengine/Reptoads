












#pragma once


#include "../gtx/fast_square_root.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_normalize_dot is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_normalize_dot extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL T normalizeDot(vec<L, T, Q> const& x, vec<L, T, Q> const& y);

	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL T fastNormalizeDot(vec<L, T, Q> const& x, vec<L, T, Q> const& y);

	
}

#include "normalize_dot.inl"
