












#pragma once


#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"
#include "../detail/_vectorize.hpp"
#include "../vector_relational.hpp"
#include "../common.hpp"
#include <limits>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_integer extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL bool isPowerOfTwo(genIUType v);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> isPowerOfTwo(vec<L, T, Q> const& v);

	
	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL genIUType ceilPowerOfTwo(genIUType v);

	
	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> ceilPowerOfTwo(vec<L, T, Q> const& v);

	
	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL genIUType floorPowerOfTwo(genIUType v);

	
	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> floorPowerOfTwo(vec<L, T, Q> const& v);

	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL genIUType roundPowerOfTwo(genIUType v);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> roundPowerOfTwo(vec<L, T, Q> const& v);

	
	
	
	template<typename genIUType>
	GLM_FUNC_DECL bool isMultiple(genIUType v, genIUType Multiple);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> isMultiple(vec<L, T, Q> const& v, T Multiple);

	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> isMultiple(vec<L, T, Q> const& v, vec<L, T, Q> const& Multiple);

	
	
	
	
	
	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType ceilMultiple(genType v, genType Multiple);

	
	
	
	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> ceilMultiple(vec<L, T, Q> const& v, vec<L, T, Q> const& Multiple);

	
	
	
	
	
	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType floorMultiple(genType v, genType Multiple);

	
	
	
	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> floorMultiple(vec<L, T, Q> const& v, vec<L, T, Q> const& Multiple);

	
	
	
	
	
	
	
	
	template<typename genType>
	GLM_FUNC_DECL genType roundMultiple(genType v, genType Multiple);

	
	
	
	
	
	
	
	
	
	
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> roundMultiple(vec<L, T, Q> const& v, vec<L, T, Q> const& Multiple);

	
} 

#include "round.inl"
