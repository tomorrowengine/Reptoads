











#pragma once


#include "../glm.hpp"
#include "../gtc/integer.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_integer is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_integer extension included")
#endif

namespace glm
{
	
	

	
	
	GLM_FUNC_DECL int pow(int x, uint y);

	
	
	GLM_FUNC_DECL int sqrt(int x);

	
	
	GLM_FUNC_DECL unsigned int floor_log2(unsigned int x);

	
	
	GLM_FUNC_DECL int mod(int x, int y);

	
	
	template<typename genType>
	GLM_FUNC_DECL genType factorial(genType const& x);

	
	
	typedef signed int					sint;

	
	
	GLM_FUNC_DECL uint pow(uint x, uint y);

	
	
	GLM_FUNC_DECL uint sqrt(uint x);

	
	
	GLM_FUNC_DECL uint mod(uint x, uint y);

	
	
	GLM_FUNC_DECL uint nlz(uint x);

	
}

#include "integer.inl"
