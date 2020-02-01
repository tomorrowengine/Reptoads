












#pragma once


#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"
#include "../detail/type_vec2.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_functions extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename T>
	GLM_FUNC_DECL T gauss(
		T x,
		T ExpectedValue,
		T StandardDeviation);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T gauss(
		vec<2, T, Q> const& Coord,
		vec<2, T, Q> const& ExpectedValue,
		vec<2, T, Q> const& StandardDeviation);

	
}

#include "functions.inl"

