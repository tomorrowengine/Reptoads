












#pragma once


#include "../detail/setup.hpp"
#include "../detail/qualifier.hpp"
#include "../vec3.hpp"
#include <limits>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_color_encoding extension included")
#endif

namespace glm
{
	
	

	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> convertLinearSRGBToD65XYZ(vec<3, T, Q> const& ColorLinearSRGB);

	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> convertLinearSRGBToD50XYZ(vec<3, T, Q> const& ColorLinearSRGB);

	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> convertD65XYZToLinearSRGB(vec<3, T, Q> const& ColorD65XYZ);

	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> convertD65XYZToD50XYZ(vec<3, T, Q> const& ColorD65XYZ);

	
} 

#include "color_encoding.inl"
