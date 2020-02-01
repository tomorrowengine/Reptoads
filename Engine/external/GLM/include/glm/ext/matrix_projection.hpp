


















#pragma once


#include "../gtc/constants.hpp"
#include "../geometric.hpp"
#include "../trigonometric.hpp"
#include "../matrix.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_projection extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> projectZO(
		vec<3, T, Q> const& obj, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	
	
	
	
	
	
	
	
	
	
	
	
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> projectNO(
		vec<3, T, Q> const& obj, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	
	
	
	
	
	
	
	
	
	
	
	
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> project(
		vec<3, T, Q> const& obj, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	
	
	
	
	
	
	
	
	
	
	
	
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> unProjectZO(
		vec<3, T, Q> const& win, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	
	
	
	
	
	
	
	
	
	
	
	
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> unProjectNO(
		vec<3, T, Q> const& win, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	
	
	
	
	
	
	
	
	
	
	
	
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> unProject(
		vec<3, T, Q> const& win, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q, typename U>
	GLM_FUNC_DECL mat<4, 4, T, Q> pickMatrix(
		vec<2, T, Q> const& center, vec<2, T, Q> const& delta, vec<4, U, Q> const& viewport);

	
}

#include "matrix_projection.inl"
