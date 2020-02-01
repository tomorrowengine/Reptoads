











#pragma once


#include "../detail/setup.hpp"
#include "../matrix.hpp"
#include "../mat2x2.hpp"
#include "../mat3x3.hpp"
#include "../mat4x4.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_matrix_inverse extension included")
#endif

namespace glm
{
	
	

	
	
	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType affineInverse(genType const& m);

	
	
	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType inverseTranspose(genType const& m);

	
}

#include "matrix_inverse.inl"
