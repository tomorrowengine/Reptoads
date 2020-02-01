











#pragma once


#include "../gtc/constants.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_fast_trigonometry is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_fast_trigonometry extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T>
	GLM_FUNC_DECL T wrapAngle(T angle);

	
	
	template<typename T>
	GLM_FUNC_DECL T fastSin(T angle);

	
	
	template<typename T>
	GLM_FUNC_DECL T fastCos(T angle);

	
	
	
	template<typename T>
	GLM_FUNC_DECL T fastTan(T angle);

	
	
	
	template<typename T>
	GLM_FUNC_DECL T fastAsin(T angle);

	
	
	
	template<typename T>
	GLM_FUNC_DECL T fastAcos(T angle);

	
	
	
	template<typename T>
	GLM_FUNC_DECL T fastAtan(T y, T x);

	
	
	
	template<typename T>
	GLM_FUNC_DECL T fastAtan(T angle);

	
}

#include "fast_trigonometry.inl"
