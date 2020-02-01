











#pragma once

#include "../detail/setup.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_scalar_int_sized extension included")
#endif

namespace glm{
namespace detail
{
#	if GLM_HAS_EXTENDED_INTEGER_TYPE
		typedef std::int8_t			int8;
		typedef std::int16_t		int16;
		typedef std::int32_t		int32;
#	else
		typedef char				int8;
		typedef short				int16;
		typedef int					int32;
#endif

	template<>
	struct is_int<int8>
	{
		enum test {value = ~0};
	};

	template<>
	struct is_int<int16>
	{
		enum test {value = ~0};
	};

	template<>
	struct is_int<int64>
	{
		enum test {value = ~0};
	};
}


	
	

	
	typedef detail::int8		int8;

	
	typedef detail::int16		int16;

	
	typedef detail::int32		int32;

	
	typedef detail::int64		int64;

	
}
