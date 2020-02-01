
















#pragma once


#include "../trigonometric.hpp"
#include "../exponential.hpp"
#include "scalar_constants.hpp"
#include "vector_relational.hpp"
#include <limits>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_quaternion_trigonometric extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T angle(qua<T, Q> const& x);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> axis(qua<T, Q> const& x);

	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> angleAxis(T const& angle, vec<3, T, Q> const& axis);

	
} 

#include "quaternion_trigonometric.inl"
