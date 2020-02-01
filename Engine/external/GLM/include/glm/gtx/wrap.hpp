











#pragma once


#include "../glm.hpp"
#include "../gtc/vec1.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_wrap is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_wrap extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename genType>
	GLM_FUNC_DECL genType clamp(genType const& Texcoord);

	
	
	template<typename genType>
	GLM_FUNC_DECL genType repeat(genType const& Texcoord);

	
	
	template<typename genType>
	GLM_FUNC_DECL genType mirrorClamp(genType const& Texcoord);

	
	
	template<typename genType>
	GLM_FUNC_DECL genType mirrorRepeat(genType const& Texcoord);

	
}

#include "wrap.inl"
