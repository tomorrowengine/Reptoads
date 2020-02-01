















#pragma once


#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"
#include "../detail/_noise.hpp"
#include "../geometric.hpp"
#include "../common.hpp"
#include "../vector_relational.hpp"
#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_noise extension included")
#endif

namespace glm
{
	
	

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL T perlin(
		vec<L, T, Q> const& p);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL T perlin(
		vec<L, T, Q> const& p,
		vec<L, T, Q> const& rep);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL T simplex(
		vec<L, T, Q> const& p);

	
}

#include "noise.inl"
