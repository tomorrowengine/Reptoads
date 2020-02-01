












#pragma once


#include "../common.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_scalar_common extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename T>
	GLM_FUNC_DECL T min(T a, T b, T c);

	
	
	
	template<typename T>
	GLM_FUNC_DECL T min(T a, T b, T c, T d);

	
	
	
	template<typename T>
	GLM_FUNC_DECL T max(T a, T b, T c);

	
	
	
	template<typename T>
	GLM_FUNC_DECL T max(T a, T b, T c, T d);

	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL T fmin(T a, T b);

	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL T fmin(T a, T b, T c);

	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL T fmin(T a, T b, T c, T d);

	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL T fmax(T a, T b);

	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL T fmax(T a, T b, T C);

	
	
	
	
	
	template<typename T>
	GLM_FUNC_DECL T fmax(T a, T b, T C, T D);

	
}

#include "scalar_common.inl"
