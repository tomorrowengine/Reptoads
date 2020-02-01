











#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_mixed_product is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_mixed_product extension included")
#endif

namespace glm
{
	
	

	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T mixedProduct(
		vec<3, T, Q> const& v1,
		vec<3, T, Q> const& v2,
		vec<3, T, Q> const& v3);

	
}

#include "mixed_product.inl"
