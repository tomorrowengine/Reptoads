











#pragma once


#include "../detail/setup.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_range is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if !GLM_HAS_RANGE_FOR
#	error "GLM_GTX_range requires C++11 suppport or 'range for'"
#endif

#include "../gtc/type_ptr.hpp"
#include "../gtc/vec1.hpp"

namespace glm
{
	
	

#	if GLM_COMPILER & GLM_COMPILER_VC
#		pragma warning(push)
#		pragma warning(disable : 4100) 
#	endif

	template<typename T, qualifier Q>
	inline length_t components(vec<1, T, Q> const& v)
	{
		return v.length();
	}

	template<typename T, qualifier Q>
	inline length_t components(vec<2, T, Q> const& v)
	{
		return v.length();
	}

	template<typename T, qualifier Q>
	inline length_t components(vec<3, T, Q> const& v)
	{
		return v.length();
	}

	template<typename T, qualifier Q>
	inline length_t components(vec<4, T, Q> const& v)
	{
		return v.length();
	}

	template<typename genType>
	inline length_t components(genType const& m)
	{
		return m.length() * m[0].length();
	}

	template<typename genType>
	inline typename genType::value_type const * begin(genType const& v)
	{
		return value_ptr(v);
	}

	template<typename genType>
	inline typename genType::value_type const * end(genType const& v)
	{
		return begin(v) + components(v);
	}

	template<typename genType>
	inline typename genType::value_type * begin(genType& v)
	{
		return value_ptr(v);
	}

	template<typename genType>
	inline typename genType::value_type * end(genType& v)
	{
		return begin(v) + components(v);
	}

#	if GLM_COMPILER & GLM_COMPILER_VC
#		pragma warning(pop)
#	endif

	
}
