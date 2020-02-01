














#pragma once


#include "../glm.hpp"
#include "../gtc/matrix_transform.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_transform is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_transform extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> translate(
		vec<3, T, Q> const& v);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> rotate(
		T angle,
		vec<3, T, Q> const& v);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> scale(
		vec<3, T, Q> const& v);

	
}

#include "transform.inl"
