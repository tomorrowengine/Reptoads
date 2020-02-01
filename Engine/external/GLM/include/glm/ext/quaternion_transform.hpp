
















#pragma once


#include "../common.hpp"
#include "../trigonometric.hpp"
#include "../geometric.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_quaternion_transform extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> rotate(qua<T, Q> const& q, T const& angle, vec<3, T, Q> const& axis);
	
} 

#include "quaternion_transform.inl"
