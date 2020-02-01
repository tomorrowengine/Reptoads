












#pragma once

#include "../detail/setup.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_scalar_float_sized extension included")
#endif

namespace glm
{
	
	

	
	typedef float			float32;


#	ifndef GLM_FORCE_SINGLE_ONLY

	
	typedef double			float64;

#	endif

	
}
