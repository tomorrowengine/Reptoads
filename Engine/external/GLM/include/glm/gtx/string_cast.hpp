















#pragma once


#include "../glm.hpp"
#include "../gtc/type_precision.hpp"
#include "../gtc/quaternion.hpp"
#include "../gtx/dual_quaternion.hpp"
#include <string>
#include <cmath>

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_string_cast is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if(GLM_COMPILER & GLM_COMPILER_CUDA)
#	error "GLM_GTX_string_cast is not supported on CUDA compiler"
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_string_cast extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename genType>
	GLM_FUNC_DECL std::string to_string(genType const& x);

	
}

#include "string_cast.inl"
