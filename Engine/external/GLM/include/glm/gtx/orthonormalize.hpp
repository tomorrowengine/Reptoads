












#pragma once


#include "../vec3.hpp"
#include "../mat3x3.hpp"
#include "../geometric.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_orthonormalize is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_orthonormalize extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> orthonormalize(mat<3, 3, T, Q> const& m);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> orthonormalize(vec<3, T, Q> const& x, vec<3, T, Q> const& y);

	
}

#include "orthonormalize.inl"
