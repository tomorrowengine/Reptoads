












#pragma once


#include "../glm.hpp"
#include "../gtx/transform.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_transform2 is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_transform2 extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> shearX2D(mat<3, 3, T, Q> const& m, T y);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> shearY2D(mat<3, 3, T, Q> const& m, T x);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> shearX3D(mat<4, 4, T, Q> const& m, T y, T z);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> shearY3D(mat<4, 4, T, Q> const& m, T x, T z);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> shearZ3D(mat<4, 4, T, Q> const& m, T x, T y);

	
	
	

	
	
	

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> proj2D(mat<3, 3, T, Q> const& m, vec<3, T, Q> const& normal);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> proj3D(mat<4, 4, T, Q> const & m, vec<3, T, Q> const& normal);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> scaleBias(T scale, T bias);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> scaleBias(mat<4, 4, T, Q> const& m, T scale, T bias);

	
}

#include "transform2.inl"
