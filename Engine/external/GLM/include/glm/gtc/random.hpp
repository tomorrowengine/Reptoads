












#pragma once


#include "../ext/scalar_int_sized.hpp"
#include "../ext/scalar_uint_sized.hpp"
#include "../detail/qualifier.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_random extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType linearRand(genType Min, genType Max);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> linearRand(vec<L, T, Q> const& Min, vec<L, T, Q> const& Max);

	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType gaussRand(genType Mean, genType Deviation);

	
	
	
	template<typename T>
	GLM_FUNC_DECL vec<2, T, defaultp> circularRand(T Radius);

	
	
	
	template<typename T>
	GLM_FUNC_DECL vec<3, T, defaultp> sphericalRand(T Radius);

	
	
	
	template<typename T>
	GLM_FUNC_DECL vec<2, T, defaultp> diskRand(T Radius);

	
	
	
	template<typename T>
	GLM_FUNC_DECL vec<3, T, defaultp> ballRand(T Radius);

	
}

#include "random.inl"
