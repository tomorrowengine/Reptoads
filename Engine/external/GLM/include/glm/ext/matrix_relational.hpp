













#pragma once


#include "../detail/qualifier.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_relational extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	
	template<length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<C, bool, Q> equal(mat<C, R, T, Q> const& x, mat<C, R, T, Q> const& y);

	
	
	
	
	
	
	
	template<length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<C, bool, Q> equal(mat<C, R, T, Q> const& x, mat<C, R, T, Q> const& y, T epsilon);

	
	
	
	
	
	
	
	template<length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<C, bool, Q> equal(mat<C, R, T, Q> const& x, mat<C, R, T, Q> const& y, vec<C, T, Q> const& epsilon);

	
	
	
	
	
	
	
	template<length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<C, bool, Q> notEqual(mat<C, R, T, Q> const& x, mat<C, R, T, Q> const& y);

	
	
	
	
	
	
	
	template<length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<C, bool, Q> notEqual(mat<C, R, T, Q> const& x, mat<C, R, T, Q> const& y, T epsilon);

	
	
	
	
	
	
	
	template<length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<C, bool, Q> notEqual(mat<C, R, T, Q> const& x, mat<C, R, T, Q> const& y, vec<C, T, Q> const& epsilon);

	
}

#include "matrix_relational.inl"
