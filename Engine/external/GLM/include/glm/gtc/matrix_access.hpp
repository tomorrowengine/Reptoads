











#pragma once


#include "../detail/setup.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_matrix_access extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename genType>
	GLM_FUNC_DECL typename genType::row_type row(
		genType const& m,
		length_t index);

	
	
	template<typename genType>
	GLM_FUNC_DECL genType row(
		genType const& m,
		length_t index,
		typename genType::row_type const& x);

	
	
	template<typename genType>
	GLM_FUNC_DECL typename genType::col_type column(
		genType const& m,
		length_t index);

	
	
	template<typename genType>
	GLM_FUNC_DECL genType column(
		genType const& m,
		length_t index,
		typename genType::col_type const& x);

	
}

#include "matrix_access.inl"
