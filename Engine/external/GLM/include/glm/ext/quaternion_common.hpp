



















#pragma once


#include "../ext/scalar_constants.hpp"
#include "../ext/quaternion_geometric.hpp"
#include "../common.hpp"
#include "../trigonometric.hpp"
#include "../exponential.hpp"
#include <limits>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_quaternion_common extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> mix(qua<T, Q> const& x, qua<T, Q> const& y, T a);

	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> lerp(qua<T, Q> const& x, qua<T, Q> const& y, T a);

	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> slerp(qua<T, Q> const& x, qua<T, Q> const& y, T a);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> conjugate(qua<T, Q> const& q);

	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL qua<T, Q> inverse(qua<T, Q> const& q);

	
	
	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> isnan(qua<T, Q> const& x);

	
	
	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, bool, Q> isinf(qua<T, Q> const& x);

	
} 

#include "quaternion_common.inl"
