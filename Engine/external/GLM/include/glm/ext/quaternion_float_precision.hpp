









#pragma once


#include "../detail/type_quat.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_quaternion_float_precision extension included")
#endif

namespace glm
{
	
	

	
	typedef qua<float, lowp>		lowp_quat;

	
	typedef qua<float, mediump>		mediump_quat;

	
	typedef qua<float, highp>		highp_quat;

	
} 

