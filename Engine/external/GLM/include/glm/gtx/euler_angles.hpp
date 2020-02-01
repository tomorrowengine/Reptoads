














#pragma once


#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_euler_angles is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_euler_angles extension included")
#endif

namespace glm
{
	
	

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleX(
		T const& angleX);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleY(
		T const& angleY);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleZ(
		T const& angleZ);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> derivedEulerAngleX(
		T const & angleX, T const & angularVelocityX);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> derivedEulerAngleY(
		T const & angleY, T const & angularVelocityY);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> derivedEulerAngleZ(
		T const & angleZ, T const & angularVelocityZ);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleXY(
		T const& angleX,
		T const& angleY);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleYX(
		T const& angleY,
		T const& angleX);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleXZ(
		T const& angleX,
		T const& angleZ);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleZX(
		T const& angle,
		T const& angleX);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleYZ(
		T const& angleY,
		T const& angleZ);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleZY(
		T const& angleZ,
		T const& angleY);

    
    
    template<typename T>
    GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleXYZ(
        T const& t1,
        T const& t2,
        T const& t3);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleYXZ(
		T const& yaw,
		T const& pitch,
		T const& roll);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleXZX(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleXYX(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleYXY(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleYZY(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleZYZ(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleZXZ(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleXZY(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleYZX(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleZYX(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> eulerAngleZXY(
		T const & t1,
		T const & t2,
		T const & t3);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> yawPitchRoll(
		T const& yaw,
		T const& pitch,
		T const& roll);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<2, 2, T, defaultp> orientate2(T const& angle);

	
	
	template<typename T>
	GLM_FUNC_DECL mat<3, 3, T, defaultp> orientate3(T const& angle);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> orientate3(vec<3, T, Q> const& angles);

	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<4, 4, T, Q> orientate4(vec<3, T, Q> const& angles);

    
    
    template<typename T>
    GLM_FUNC_DECL void extractEulerAngleXYZ(mat<4, 4, T, defaultp> const& M,
                                            T & t1,
                                            T & t2,
                                            T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleYXZ(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleXZX(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleXYX(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleYXY(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleYZY(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleZYZ(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleZXZ(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleXZY(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleYZX(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleZYX(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
	
	template <typename T>
	GLM_FUNC_DECL void extractEulerAngleZXY(mat<4, 4, T, defaultp> const & M,
											T & t1,
											T & t2,
											T & t3);

	
}

#include "euler_angles.inl"
