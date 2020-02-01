









#pragma once


#include "../detail/type_quat.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_quaternion_double_precision extension included")
#endif

namespace glm
{
	
	

	
	
	
	typedef qua<double, lowp>		lowp_dquat;

	
	
	
	typedef qua<double, mediump>	mediump_dquat;

	
	
	
	typedef qua<double, highp>		highp_dquat;

	
} 

