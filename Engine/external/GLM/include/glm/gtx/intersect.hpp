












#pragma once


#include <cfloat>
#include <limits>
#include "../glm.hpp"
#include "../geometric.hpp"
#include "../gtx/closest_point.hpp"
#include "../gtx/vector_query.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_closest_point is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_closest_point extension included")
#endif

namespace glm
{
	
	

	
	
	
	template<typename genType>
	GLM_FUNC_DECL bool intersectRayPlane(
		genType const& orig, genType const& dir,
		genType const& planeOrig, genType const& planeNormal,
		typename genType::value_type & intersectionDistance);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL bool intersectRayTriangle(
		vec<3, T, Q> const& orig, vec<3, T, Q> const& dir,
		vec<3, T, Q> const& v0, vec<3, T, Q> const& v1, vec<3, T, Q> const& v2,
		vec<2, T, Q>& baryPosition, T& distance);

	
	
	template<typename genType>
	GLM_FUNC_DECL bool intersectLineTriangle(
		genType const& orig, genType const& dir,
		genType const& vert0, genType const& vert1, genType const& vert2,
		genType & position);

	
	
	
	template<typename genType>
	GLM_FUNC_DECL bool intersectRaySphere(
		genType const& rayStarting, genType const& rayNormalizedDirection,
		genType const& sphereCenter, typename genType::value_type const sphereRadiusSquered,
		typename genType::value_type & intersectionDistance);

	
	
	template<typename genType>
	GLM_FUNC_DECL bool intersectRaySphere(
		genType const& rayStarting, genType const& rayNormalizedDirection,
		genType const& sphereCenter, const typename genType::value_type sphereRadius,
		genType & intersectionPosition, genType & intersectionNormal);

	
	
	template<typename genType>
	GLM_FUNC_DECL bool intersectLineSphere(
		genType const& point0, genType const& point1,
		genType const& sphereCenter, typename genType::value_type sphereRadius,
		genType & intersectionPosition1, genType & intersectionNormal1,
		genType & intersectionPosition2 = genType(), genType & intersectionNormal2 = genType());

	
}

#include "intersect.inl"
