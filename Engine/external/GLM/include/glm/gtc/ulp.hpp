













#pragma once


#include "../gtc/constants.hpp"
#include "../ext/vector_relational.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_ulp extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename genType>
	GLM_FUNC_DECL genType next_float(genType const& x);

	
	
	template<typename genType>
	GLM_FUNC_DECL genType prev_float(genType const& x);

	
	
	template<typename genType>
	GLM_FUNC_DECL genType next_float(genType const& x, uint const& Distance);

	
	
	template<typename genType>
	GLM_FUNC_DECL genType prev_float(genType const& x, uint const& Distance);

	
	
	template<typename T>
	GLM_FUNC_DECL uint float_distance(T const& x, T const& y);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, uint, Q> float_distance(vec<2, T, Q> const& x, vec<2, T, Q> const& y);

	
}

#include "ulp.inl"
