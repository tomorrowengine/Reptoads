











#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_matrix_factorisation is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_matrix_factorisation extension included")
#endif



namespace glm
{
	
	

	
	
	
	template <length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL mat<C, R, T, Q> flipud(mat<C, R, T, Q> const& in);

	
	
	
	template <length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL mat<C, R, T, Q> fliplr(mat<C, R, T, Q> const& in);

	
	
	
	
	
	template <length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL void qr_decompose(mat<C, R, T, Q> const& in, mat<(C < R ? C : R), R, T, Q>& q, mat<C, (C < R ? C : R), T, Q>& r);

	
	
	
	
	
	
	template <length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL void rq_decompose(mat<C, R, T, Q> const& in, mat<(C < R ? C : R), R, T, Q>& r, mat<C, (C < R ? C : R), T, Q>& q);

	
}

#include "matrix_factorisation.inl"
