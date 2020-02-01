












#pragma once


#include "../gtc/constants.hpp"
#include "../gtc/matrix_transform.hpp"
#include "../ext/vector_relational.hpp"
#include "../ext/quaternion_common.hpp"
#include "../ext/quaternion_float.hpp"
#include "../ext/quaternion_float_precision.hpp"
#include "../ext/quaternion_double.hpp"
#include "../ext/quaternion_double_precision.hpp"
#include "../ext/quaternion_relational.hpp"
#include "../ext/quaternion_geometric.hpp"
#include "../ext/quaternion_trigonometric.hpp"
#include "../ext/quaternion_transform.hpp"
#include "../detail/type_mat3x3.hpp"
#include "../detail/type_mat4x4.hpp"
#include "../detail/type_vec3.hpp"
#include "../detail/type_vec4.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_quaternion extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> eulerAngles(qua<T, Q> const& x);

	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T roll(qua<T, Q> const& x);

	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T pitch(qua<T, Q> const& x);

	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T yaw(qua<T, Q> const& x);

	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> mat3_cast(qua<T, Q> const& x);

	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> mat4_cast(qua<T, Q> const& x);

	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> quat_cast(mat<3, 3, T, Q> const& x);

	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> quat_cast(mat<4, 4, T, Q> const& x);

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> lessThan(qua<T, Q> const& x, qua<T, Q> const& y);

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> lessThanEqual(qua<T, Q> const& x, qua<T, Q> const& y);

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> greaterThan(qua<T, Q> const& x, qua<T, Q> const& y);

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> greaterThanEqual(qua<T, Q> const& x, qua<T, Q> const& y);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> quatLookAt(
		vec<3, T, Q> const& direction,
		vec<3, T, Q> const& up);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> quatLookAtRH(
		vec<3, T, Q> const& direction,
		vec<3, T, Q> const& up);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> quatLookAtLH(
		vec<3, T, Q> const& direction,
		vec<3, T, Q> const& up);
	
} 

#include "quaternion.inl"
