












#pragma once


#include "../glm.hpp"
#include "../gtc/constants.hpp"
#include "../gtc/quaternion.hpp"
#include "../ext/quaternion_exponential.hpp"
#include "../gtx/norm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_quaternion is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_quaternion extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> quat_identity();

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> cross(
		qua<T, Q> const& q,
		vec<3, T, Q> const& v);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> cross(
		vec<3, T, Q> const& v,
		qua<T, Q> const& q);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> squad(
		qua<T, Q> const& q1,
		qua<T, Q> const& q2,
		qua<T, Q> const& s1,
		qua<T, Q> const& s2,
		T const& h);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> intermediate(
		qua<T, Q> const& prev,
		qua<T, Q> const& curr,
		qua<T, Q> const& next);

	
	
	
	
	
	

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> rotate(
		qua<T, Q> const& q,
		vec<3, T, Q> const& v);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> rotate(
		qua<T, Q> const& q,
		vec<4, T, Q> const& v);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T extractRealComponent(
		qua<T, Q> const& q);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> toMat3(
		qua<T, Q> const& x){return mat3_cast(x);}

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> toMat4(
		qua<T, Q> const& x){return mat4_cast(x);}

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> toQuat(
		mat<3, 3, T, Q> const& x){return quat_cast(x);}

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> toQuat(
		mat<4, 4, T, Q> const& x){return quat_cast(x);}

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> shortMix(
		qua<T, Q> const& x,
		qua<T, Q> const& y,
		T const& a);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> fastMix(
		qua<T, Q> const& x,
		qua<T, Q> const& y,
		T const& a);

	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> rotation(
		vec<3, T, Q> const& orig,
		vec<3, T, Q> const& dest);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T length2(qua<T, Q> const& q);

	
}

#include "quaternion.inl"
