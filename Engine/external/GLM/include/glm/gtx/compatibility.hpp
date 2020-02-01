











#pragma once


#include "../glm.hpp"
#include "../gtc/quaternion.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_compatibility is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_compatibility extension included")
#endif

#if GLM_COMPILER & GLM_COMPILER_VC
#	include <cfloat>
#elif GLM_COMPILER & GLM_COMPILER_GCC
#	include <cmath>
#	if(GLM_PLATFORM & GLM_PLATFORM_ANDROID)
#		undef isfinite
#	endif
#endif

namespace glm
{
	
	

	template<typename T> GLM_FUNC_QUALIFIER T lerp(T x, T y, T a){return mix(x, y, a);}																					
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<2, T, Q> lerp(const vec<2, T, Q>& x, const vec<2, T, Q>& y, T a){return mix(x, y, a);}							

	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<3, T, Q> lerp(const vec<3, T, Q>& x, const vec<3, T, Q>& y, T a){return mix(x, y, a);}							
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<4, T, Q> lerp(const vec<4, T, Q>& x, const vec<4, T, Q>& y, T a){return mix(x, y, a);}							
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<2, T, Q> lerp(const vec<2, T, Q>& x, const vec<2, T, Q>& y, const vec<2, T, Q>& a){return mix(x, y, a);}	
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<3, T, Q> lerp(const vec<3, T, Q>& x, const vec<3, T, Q>& y, const vec<3, T, Q>& a){return mix(x, y, a);}	
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<4, T, Q> lerp(const vec<4, T, Q>& x, const vec<4, T, Q>& y, const vec<4, T, Q>& a){return mix(x, y, a);}	

	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER T saturate(T x){return clamp(x, T(0), T(1));}														
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<2, T, Q> saturate(const vec<2, T, Q>& x){return clamp(x, T(0), T(1));}					
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<3, T, Q> saturate(const vec<3, T, Q>& x){return clamp(x, T(0), T(1));}					
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<4, T, Q> saturate(const vec<4, T, Q>& x){return clamp(x, T(0), T(1));}					

	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER T atan2(T x, T y){return atan(x, y);}																
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<2, T, Q> atan2(const vec<2, T, Q>& x, const vec<2, T, Q>& y){return atan(x, y);}	
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<3, T, Q> atan2(const vec<3, T, Q>& x, const vec<3, T, Q>& y){return atan(x, y);}	
	template<typename T, qualifier Q> GLM_FUNC_QUALIFIER vec<4, T, Q> atan2(const vec<4, T, Q>& x, const vec<4, T, Q>& y){return atan(x, y);}	

	template<typename genType> GLM_FUNC_DECL bool isfinite(genType const& x);											
	template<typename T, qualifier Q> GLM_FUNC_DECL vec<1, bool, Q> isfinite(const vec<1, T, Q>& x);				
	template<typename T, qualifier Q> GLM_FUNC_DECL vec<2, bool, Q> isfinite(const vec<2, T, Q>& x);				
	template<typename T, qualifier Q> GLM_FUNC_DECL vec<3, bool, Q> isfinite(const vec<3, T, Q>& x);				
	template<typename T, qualifier Q> GLM_FUNC_DECL vec<4, bool, Q> isfinite(const vec<4, T, Q>& x);				

	typedef bool						bool1;			
	typedef vec<2, bool, highp>			bool2;			
	typedef vec<3, bool, highp>			bool3;			
	typedef vec<4, bool, highp>			bool4;			

	typedef bool						bool1x1;		
	typedef mat<2, 2, bool, highp>		bool2x2;		
	typedef mat<2, 3, bool, highp>		bool2x3;		
	typedef mat<2, 4, bool, highp>		bool2x4;		
	typedef mat<3, 2, bool, highp>		bool3x2;		
	typedef mat<3, 3, bool, highp>		bool3x3;		
	typedef mat<3, 4, bool, highp>		bool3x4;		
	typedef mat<4, 2, bool, highp>		bool4x2;		
	typedef mat<4, 3, bool, highp>		bool4x3;		
	typedef mat<4, 4, bool, highp>		bool4x4;		

	typedef int							int1;			
	typedef vec<2, int, highp>			int2;			
	typedef vec<3, int, highp>			int3;			
	typedef vec<4, int, highp>			int4;			

	typedef int							int1x1;			
	typedef mat<2, 2, int, highp>		int2x2;			
	typedef mat<2, 3, int, highp>		int2x3;			
	typedef mat<2, 4, int, highp>		int2x4;			
	typedef mat<3, 2, int, highp>		int3x2;			
	typedef mat<3, 3, int, highp>		int3x3;			
	typedef mat<3, 4, int, highp>		int3x4;			
	typedef mat<4, 2, int, highp>		int4x2;			
	typedef mat<4, 3, int, highp>		int4x3;			
	typedef mat<4, 4, int, highp>		int4x4;			

	typedef float						float1;			
	typedef vec<2, float, highp>		float2;			
	typedef vec<3, float, highp>		float3;			
	typedef vec<4, float, highp>		float4;			

	typedef float						float1x1;		
	typedef mat<2, 2, float, highp>		float2x2;		
	typedef mat<2, 3, float, highp>		float2x3;		
	typedef mat<2, 4, float, highp>		float2x4;		
	typedef mat<3, 2, float, highp>		float3x2;		
	typedef mat<3, 3, float, highp>		float3x3;		
	typedef mat<3, 4, float, highp>		float3x4;		
	typedef mat<4, 2, float, highp>		float4x2;		
	typedef mat<4, 3, float, highp>		float4x3;		
	typedef mat<4, 4, float, highp>		float4x4;		

	typedef double						double1;		
	typedef vec<2, double, highp>		double2;		
	typedef vec<3, double, highp>		double3;		
	typedef vec<4, double, highp>		double4;		

	typedef double						double1x1;		
	typedef mat<2, 2, double, highp>		double2x2;		
	typedef mat<2, 3, double, highp>		double2x3;		
	typedef mat<2, 4, double, highp>		double2x4;		
	typedef mat<3, 2, double, highp>		double3x2;		
	typedef mat<3, 3, double, highp>		double3x3;		
	typedef mat<3, 4, double, highp>		double3x4;		
	typedef mat<4, 2, double, highp>		double4x2;		
	typedef mat<4, 3, double, highp>		double4x3;		
	typedef mat<4, 4, double, highp>		double4x4;		

	
}

#include "compatibility.inl"
