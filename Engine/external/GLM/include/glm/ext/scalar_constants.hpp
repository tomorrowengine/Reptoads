









#pragma once


#include "../detail/setup.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_scalar_constants extension included")
#endif

namespace glm
{
	
	

	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType epsilon();

	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType pi();

	
} 

#include "scalar_constants.inl"
