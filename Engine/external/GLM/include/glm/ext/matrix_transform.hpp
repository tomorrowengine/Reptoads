


















#pragma once


#include "../gtc/constants.hpp"
#include "../geometric.hpp"
#include "../trigonometric.hpp"
#include "../matrix.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_transform extension included")
#endif

namespace glm
{
	
	

	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType identity();

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> translate(
		mat<4, 4, T, Q> const& m, vec<3, T, Q> const& v);

	
	
	
	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> rotate(
		mat<4, 4, T, Q> const& m, T angle, vec<3, T, Q> const& axis);

	
	
	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> scale(
		mat<4, 4, T, Q> const& m, vec<3, T, Q> const& v);

	
	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> lookAtRH(
		vec<3, T, Q> const& eye, vec<3, T, Q> const& center, vec<3, T, Q> const& up);

	
	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> lookAtLH(
		vec<3, T, Q> const& eye, vec<3, T, Q> const& center, vec<3, T, Q> const& up);

	
	
	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> lookAt(
		vec<3, T, Q> const& eye, vec<3, T, Q> const& center, vec<3, T, Q> const& up);

	
}

#include "matrix_transform.inl"
