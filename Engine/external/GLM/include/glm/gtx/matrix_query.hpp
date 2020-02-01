












#pragma once


#include "../glm.hpp"
#include "../gtx/vector_query.hpp"
#include <limits>

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_matrix_query is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_matrix_query extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL bool isNull(mat<2, 2, T, Q> const& m, T const& epsilon);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL bool isNull(mat<3, 3, T, Q> const& m, T const& epsilon);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL bool isNull(mat<4, 4, T, Q> const& m, T const& epsilon);

	
	
	template<length_t C, length_t R, typename T, qualifier Q, template<length_t, length_t, typename, qualifier> class matType>
	GLM_FUNC_DECL bool isIdentity(matType<C, R, T, Q> const& m, T const& epsilon);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL bool isNormalized(mat<2, 2, T, Q> const& m, T const& epsilon);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL bool isNormalized(mat<3, 3, T, Q> const& m, T const& epsilon);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL bool isNormalized(mat<4, 4, T, Q> const& m, T const& epsilon);

	
	
	template<length_t C, length_t R, typename T, qualifier Q, template<length_t, length_t, typename, qualifier> class matType>
	GLM_FUNC_DECL bool isOrthogonal(matType<C, R, T, Q> const& m, T const& epsilon);

	
}

#include "matrix_query.inl"
