











#pragma once


#include "../geometric.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_projection is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_projection extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType proj(genType const& x, genType const& Normal);

	
}

#include "projection.inl"
