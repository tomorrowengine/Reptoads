













#pragma once


#include "../geometric.hpp"
#include "../exponential.hpp"
#include "../ext/vector_relational.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_quaternion_geometric extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T length(qua<T, Q> const& q);

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> normalize(qua<T, Q> const& q);

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T dot(qua<T, Q> const& x, qua<T, Q> const& y);

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER qua<T, Q> cross(qua<T, Q> const& q1, qua<T, Q> const& q2);

	
} 

#include "quaternion_geometric.inl"
