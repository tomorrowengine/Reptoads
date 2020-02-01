












#pragma once


#include "../glm.hpp"
#include <cstdlib>

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_std_based_type is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_std_based_type extension included")
#endif

namespace glm
{
	
	

	
	
	typedef vec<1, std::size_t, defaultp>		size1;

	
	
	typedef vec<2, std::size_t, defaultp>		size2;

	
	
	typedef vec<3, std::size_t, defaultp>		size3;

	
	
	typedef vec<4, std::size_t, defaultp>		size4;

	
	
	typedef vec<1, std::size_t, defaultp>		size1_t;

	
	
	typedef vec<2, std::size_t, defaultp>		size2_t;

	
	
	typedef vec<3, std::size_t, defaultp>		size3_t;

	
	
	typedef vec<4, std::size_t, defaultp>		size4_t;

	
}

#include "std_based_type.inl"
