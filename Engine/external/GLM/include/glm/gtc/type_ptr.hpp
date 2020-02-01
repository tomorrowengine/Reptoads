
































#pragma once


#include "../gtc/quaternion.hpp"
#include "../gtc/vec1.hpp"
#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include "../mat2x2.hpp"
#include "../mat2x3.hpp"
#include "../mat2x4.hpp"
#include "../mat3x2.hpp"
#include "../mat3x3.hpp"
#include "../mat3x4.hpp"
#include "../mat4x2.hpp"
#include "../mat4x3.hpp"
#include "../mat4x4.hpp"
#include <cstring>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_type_ptr extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename genType>
	GLM_FUNC_DECL typename genType::value_type const * value_ptr(genType const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<1, T, Q> make_vec1(vec<1, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<1, T, Q> make_vec1(vec<2, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<1, T, Q> make_vec1(vec<3, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<1, T, Q> make_vec1(vec<4, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> make_vec2(vec<1, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> make_vec2(vec<2, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> make_vec2(vec<3, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> make_vec2(vec<4, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> make_vec3(vec<1, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> make_vec3(vec<2, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> make_vec3(vec<3, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> make_vec3(vec<4, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> make_vec4(vec<1, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> make_vec4(vec<2, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> make_vec4(vec<3, T, Q> const& v);

	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> make_vec4(vec<4, T, Q> const& v);

	
	
	template<typename T>
	GLM_FUNC_DECL vec<2, T, defaultp> make_vec2(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL vec<3, T, defaultp> make_vec3(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL vec<4, T, defaultp> make_vec4(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<2, 2, T, defaultp> make_mat2x2(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<2, 3, T, defaultp> make_mat2x3(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<2, 4, T, defaultp> make_mat2x4(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<3, 2, T, defaultp> make_mat3x2(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<3, 3, T, defaultp> make_mat3x3(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<3, 4, T, defaultp> make_mat3x4(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 2, T, defaultp> make_mat4x2(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 3, T, defaultp> make_mat4x3(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> make_mat4x4(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<2, 2, T, defaultp> make_mat2(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<3, 3, T, defaultp> make_mat3(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> make_mat4(T const * const ptr);

	
	
	template<typename T>
	GLM_FUNC_DECL qua<T, defaultp> make_quat(T const * const ptr);

	
}

#include "type_ptr.inl"
