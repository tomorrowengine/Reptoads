












#pragma once


#include "../ext/scalar_common.hpp"
#include "../common.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_vector_common extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<L, T, Q> min(vec<L, T, Q> const& a, vec<L, T, Q> const& b, vec<L, T, Q> const& c);

	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<L, T, Q> min(vec<L, T, Q> const& a, vec<L, T, Q> const& b, vec<L, T, Q> const& c, vec<L, T, Q> const& d);

	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<L, T, Q> max(vec<L, T, Q> const& x, vec<L, T, Q> const& y, vec<L, T, Q> const& z);

	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<L, T, Q> max( vec<L, T, Q> const& x, vec<L, T, Q> const& y, vec<L, T, Q> const& z, vec<L, T, Q> const& w);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fmin(vec<L, T, Q> const& x, T y);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fmin(vec<L, T, Q> const& x, vec<L, T, Q> const& y);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fmin(vec<L, T, Q> const& a, vec<L, T, Q> const& b, vec<L, T, Q> const& c);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fmin(vec<L, T, Q> const& a, vec<L, T, Q> const& b, vec<L, T, Q> const& c, vec<L, T, Q> const& d);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fmax(vec<L, T, Q> const& a, T b);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fmax(vec<L, T, Q> const& a, vec<L, T, Q> const& b);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fmax(vec<L, T, Q> const& a, vec<L, T, Q> const& b, vec<L, T, Q> const& c);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fmax(vec<L, T, Q> const& a, vec<L, T, Q> const& b, vec<L, T, Q> const& c, vec<L, T, Q> const& d);

	
}

#include "vector_common.inl"
