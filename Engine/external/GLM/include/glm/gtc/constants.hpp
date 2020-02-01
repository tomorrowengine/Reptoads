











#pragma once


#include "../ext/scalar_constants.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_constants extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType zero();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType one();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType two_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType root_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType half_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType three_over_two_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType quarter_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType one_over_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType one_over_two_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType two_over_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType four_over_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType two_over_root_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType one_over_root_two();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType root_half_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType root_two_pi();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType root_ln_four();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType e();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType euler();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType root_two();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType root_three();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType root_five();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType ln_two();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType ln_ten();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType ln_ln_two();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType third();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType two_thirds();

	
	
	template<typename genType>
	GLM_FUNC_DECL GLM_CONSTEXPR genType golden_ratio();

	
} 

#include "constants.inl"
