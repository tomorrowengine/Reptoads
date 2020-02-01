












#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_fast_exponential is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_fast_exponential extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename genType>
	GLM_FUNC_DECL genType fastPow(genType x, genType y);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fastPow(vec<L, T, Q> const& x, vec<L, T, Q> const& y);

	
	
	template<typename genTypeT, typename genTypeU>
	GLM_FUNC_DECL genTypeT fastPow(genTypeT x, genTypeU y);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fastPow(vec<L, T, Q> const& x);

	
	
	template<typename T>
	GLM_FUNC_DECL T fastExp(T x);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fastExp(vec<L, T, Q> const& x);

	
	
	template<typename T>
	GLM_FUNC_DECL T fastLog(T x);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fastLog(vec<L, T, Q> const& x);

	
	
	template<typename T>
	GLM_FUNC_DECL T fastExp2(T x);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fastExp2(vec<L, T, Q> const& x);

	
	
	template<typename T>
	GLM_FUNC_DECL T fastLog2(T x);

	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> fastLog2(vec<L, T, Q> const& x);

	
}

#include "fast_exponential.inl"
