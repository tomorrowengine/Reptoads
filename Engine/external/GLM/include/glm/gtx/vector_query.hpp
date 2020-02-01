











#pragma once


#include "../glm.hpp"
#include <cfloat>
#include <limits>

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_vector_query is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_vector_query extension included")
#endif

namespace glm
{
	
	

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL bool areCollinear(vec<L, T, Q> const& v0, vec<L, T, Q> const& v1, T const& epsilon);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL bool areOrthogonal(vec<L, T, Q> const& v0, vec<L, T, Q> const& v1, T const& epsilon);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL bool isNormalized(vec<L, T, Q> const& v, T const& epsilon);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL bool isNull(vec<L, T, Q> const& v, T const& epsilon);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> isCompNull(vec<L, T, Q> const& v, T const& epsilon);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL bool areOrthonormal(vec<L, T, Q> const& v0, vec<L, T, Q> const& v1, T const& epsilon);

	
}

#include "vector_query.inl"
