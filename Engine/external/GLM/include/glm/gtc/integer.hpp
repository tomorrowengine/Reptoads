












#pragma once


#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"
#include "../common.hpp"
#include "../integer.hpp"
#include "../exponential.hpp"
#include <limits>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_integer extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename genIUType>
	GLM_FUNC_DECL genIUType log2(genIUType x);

	
	
	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, int, Q> iround(vec<L, T, Q> const& x);

	
	
	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, uint, Q> uround(vec<L, T, Q> const& x);

	
} 

#include "integer.inl"
