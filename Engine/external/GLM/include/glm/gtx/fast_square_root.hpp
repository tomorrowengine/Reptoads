













#pragma once


#include "../common.hpp"
#include "../exponential.hpp"
#include "../geometric.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_fast_square_root is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_fast_square_root extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType fastSqrt(genType x);

	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fastSqrt(vec<L, T, Q> const& x);

	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType fastInverseSqrt(genType x);

	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fastInverseSqrt(vec<L, T, Q> const& x);

	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType fastLength(genType x);

	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL T fastLength(vec<L, T, Q> const& x);

	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType fastDistance(genType x, genType y);

	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL T fastDistance(vec<L, T, Q> const& x, vec<L, T, Q> const& y);

	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType fastNormalize(genType const& x);

	
}

#include "fast_square_root.inl"
