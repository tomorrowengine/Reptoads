













#pragma once


#include "../common.hpp"
#include "../trigonometric.hpp"
#include "../geometric.hpp"
#include "../ext/scalar_constants.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_quaternion_exponential extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> exp(qua<T, Q> const& q);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> log(qua<T, Q> const& q);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> pow(qua<T, Q> const& q, T y);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> sqrt(qua<T, Q> const& q);

	
} 

#include "quaternion_exponential.inl"
