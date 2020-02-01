















#pragma once


#include "../glm.hpp"
#include "../gtc/constants.hpp"
#include "../detail/qualifier.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_easing is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_easing extension included")
#endif

namespace glm{
	
	

	
	
	template <typename genType>
	GLM_FUNC_DECL genType linearInterpolation(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType quadraticEaseIn(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType quadraticEaseOut(genType const & a);

	
	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType quadraticEaseInOut(genType const & a);

	
	template <typename genType>
	GLM_FUNC_DECL genType cubicEaseIn(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType cubicEaseOut(genType const & a);

	
	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType cubicEaseInOut(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType quarticEaseIn(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType quarticEaseOut(genType const & a);

	
	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType quarticEaseInOut(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType quinticEaseIn(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType quinticEaseOut(genType const & a);

	
	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType quinticEaseInOut(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType sineEaseIn(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType sineEaseOut(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType sineEaseInOut(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType circularEaseIn(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType circularEaseOut(genType const & a);

	
	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType circularEaseInOut(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType exponentialEaseIn(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType exponentialEaseOut(genType const & a);

	
	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType exponentialEaseInOut(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType elasticEaseIn(genType const & a);

	
	
	template <typename genType>
	GLM_FUNC_DECL genType elasticEaseOut(genType const & a);

	
	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType elasticEaseInOut(genType const & a);

	
	template <typename genType>
	GLM_FUNC_DECL genType backEaseIn(genType const& a);

	
	template <typename genType>
	GLM_FUNC_DECL genType backEaseOut(genType const& a);

	
	template <typename genType>
	GLM_FUNC_DECL genType backEaseInOut(genType const& a);

	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType backEaseIn(genType const& a, genType const& o);

	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType backEaseOut(genType const& a, genType const& o);

	
	
	
	template <typename genType>
	GLM_FUNC_DECL genType backEaseInOut(genType const& a, genType const& o);

	
	template <typename genType>
	GLM_FUNC_DECL genType bounceEaseIn(genType const& a);

	
	template <typename genType>
	GLM_FUNC_DECL genType bounceEaseOut(genType const& a);

	
	template <typename genType>
	GLM_FUNC_DECL genType bounceEaseInOut(genType const& a);

	
}

#include "easing.inl"
