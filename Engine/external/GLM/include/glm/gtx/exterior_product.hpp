












#pragma once


#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_exterior_product extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T cross(vec<2, T, Q> const& v, vec<2, T, Q> const& u);

	
} 

#include "exterior_product.inl"
