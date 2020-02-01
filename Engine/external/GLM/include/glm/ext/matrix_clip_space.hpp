


















#pragma once


#include "../ext/scalar_constants.hpp"
#include "../geometric.hpp"
#include "../trigonometric.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_clip_space extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> ortho(
		T left, T right, T bottom, T top);

	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> orthoLH_ZO(
		T left, T right, T bottom, T top, T zNear, T zFar);

	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> orthoLH_NO(
		T left, T right, T bottom, T top, T zNear, T zFar);

	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> orthoRH_ZO(
		T left, T right, T bottom, T top, T zNear, T zFar);

	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> orthoRH_NO(
		T left, T right, T bottom, T top, T zNear, T zFar);

	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> orthoZO(
		T left, T right, T bottom, T top, T zNear, T zFar);

	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> orthoNO(
		T left, T right, T bottom, T top, T zNear, T zFar);

	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> orthoLH(
		T left, T right, T bottom, T top, T zNear, T zFar);

	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> orthoRH(
		T left, T right, T bottom, T top, T zNear, T zFar);

	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> ortho(
		T left, T right, T bottom, T top, T zNear, T zFar);

	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> frustumLH_ZO(
		T left, T right, T bottom, T top, T near, T far);

	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> frustumLH_NO(
		T left, T right, T bottom, T top, T near, T far);

	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> frustumRH_ZO(
		T left, T right, T bottom, T top, T near, T far);

	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> frustumRH_NO(
		T left, T right, T bottom, T top, T near, T far);

	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> frustumZO(
		T left, T right, T bottom, T top, T near, T far);

	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> frustumNO(
		T left, T right, T bottom, T top, T near, T far);

	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> frustumLH(
		T left, T right, T bottom, T top, T near, T far);

	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> frustumRH(
		T left, T right, T bottom, T top, T near, T far);

	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> frustum(
		T left, T right, T bottom, T top, T near, T far);


	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveRH_ZO(
		T fovy, T aspect, T near, T far);

	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveRH_NO(
		T fovy, T aspect, T near, T far);

	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveLH_ZO(
		T fovy, T aspect, T near, T far);

	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveLH_NO(
		T fovy, T aspect, T near, T far);

	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveZO(
		T fovy, T aspect, T near, T far);

	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveNO(
		T fovy, T aspect, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveRH(
		T fovy, T aspect, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveLH(
		T fovy, T aspect, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspective(
		T fovy, T aspect, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveFovRH_ZO(
		T fov, T width, T height, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveFovRH_NO(
		T fov, T width, T height, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveFovLH_ZO(
		T fov, T width, T height, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveFovLH_NO(
		T fov, T width, T height, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveFovZO(
		T fov, T width, T height, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveFovNO(
		T fov, T width, T height, T near, T far);

	
	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveFovRH(
		T fov, T width, T height, T near, T far);

	
	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveFovLH(
		T fov, T width, T height, T near, T far);

	
	
	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> perspectiveFov(
		T fov, T width, T height, T near, T far);

	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> infinitePerspectiveLH(
		T fovy, T aspect, T near);

	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> infinitePerspectiveRH(
		T fovy, T aspect, T near);

	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> infinitePerspective(
		T fovy, T aspect, T near);

	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> tweakedInfinitePerspective(
		T fovy, T aspect, T near);

	
	
	
	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> tweakedInfinitePerspective(
		T fovy, T aspect, T near, T ep);

	
}

#include "matrix_clip_space.inl"
