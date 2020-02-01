













#pragma once


#include "../detail/qualifier.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_scalar_relational extension included")
#endif

namespace glm
{
	
	
	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR bool equal(genType const& x, genType const& y, genType const& epsilon);

	
	
	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR bool notEqual(genType const& x, genType const& y, genType const& epsilon);

	
}

#include "scalar_relational.inl"
