











#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_extend is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_extend extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename genType>
	GLM_FUNC_DECL genType extend(
		genType const& Origin,
		genType const& Source,
		typename genType::value_type const Length);

	
}

#include "extend.inl"
