












#pragma once


#include "../glm.hpp"
#include "../gtx/optimum_pow.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_gradient_paint is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_gradient_paint extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T radialGradient(
		vec<2, T, Q> const& Center,
		T const& Radius,
		vec<2, T, Q> const& Focal,
		vec<2, T, Q> const& Position);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T linearGradient(
		vec<2, T, Q> const& Point0,
		vec<2, T, Q> const& Point1,
		vec<2, T, Q> const& Position);

	
}

#include "gradient_paint.inl"
