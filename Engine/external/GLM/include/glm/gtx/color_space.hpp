











#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_color_space is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_color_space extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> rgbColor(
		vec<3, T, Q> const& hsvValue);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> hsvColor(
		vec<3, T, Q> const& rgbValue);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> saturation(
		T const s);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> saturation(
		T const s,
		vec<3, T, Q> const& color);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> saturation(
		T const s,
		vec<4, T, Q> const& color);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL T luminosity(
		vec<3, T, Q> const& color);

	
}

#include "color_space.inl"
