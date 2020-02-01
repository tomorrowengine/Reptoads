











#pragma once


#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include "../gtc/vec1.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_common is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_common extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	
	
	template<typename genType>
	GLM_FUNC_DECL typename genType::bool_type isdenormal(genType const& x);

	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fmod(vec<L, T, Q> const& v);

	
	
	
	
	
	
	
	template <length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> openBounded(vec<L, T, Q> const& Value, vec<L, T, Q> const& Min, vec<L, T, Q> const& Max);

	
	
	
	
	
	
	
	template <length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> closeBounded(vec<L, T, Q> const& Value, vec<L, T, Q> const& Min, vec<L, T, Q> const& Max);

	
}

#include "common.inl"
