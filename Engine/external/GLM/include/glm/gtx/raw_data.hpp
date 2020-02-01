











#pragma once


#include "../ext/scalar_uint_sized.hpp"
#include "../detail/setup.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_raw_data is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_raw_data extension included")
#endif

namespace glm
{
	
	

	
	
	typedef detail::uint8		byte;

	
	
	typedef detail::uint16		word;

	
	
	typedef detail::uint32		dword;

	
	
	typedef detail::uint64		qword;

	
}

#include "raw_data.inl"
