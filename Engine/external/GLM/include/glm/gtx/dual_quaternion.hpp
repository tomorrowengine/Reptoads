














#pragma once


#include "../glm.hpp"
#include "../gtc/constants.hpp"
#include "../gtc/quaternion.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_dual_quaternion is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_dual_quaternion extension included")
#endif

namespace glm
{
	
	

	template<typename T, qualifier Q = defaultp>
	struct tdualquat
	{
		

		typedef T value_type;
		typedef qua<T, Q> part_type;

		

		qua<T, Q> real, dual;

		

		typedef length_t length_type;
		
		GLM_FUNC_DECL static GLM_CONSTEXPR length_type length(){return 2;}

		GLM_FUNC_DECL part_type & operator[](length_type i);
		GLM_FUNC_DECL part_type const& operator[](length_type i) const;

		

		GLM_FUNC_DECL GLM_CONSTEXPR tdualquat() GLM_DEFAULT;
		GLM_FUNC_DECL GLM_CONSTEXPR tdualquat(tdualquat<T, Q> const& d) GLM_DEFAULT;
		template<qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR tdualquat(tdualquat<T, P> const& d);

		

		GLM_FUNC_DECL GLM_CONSTEXPR tdualquat(qua<T, Q> const& real);
		GLM_FUNC_DECL GLM_CONSTEXPR tdualquat(qua<T, Q> const& orientation, vec<3, T, Q> const& translation);
		GLM_FUNC_DECL GLM_CONSTEXPR tdualquat(qua<T, Q> const& real, qua<T, Q> const& dual);

		

		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT tdualquat(tdualquat<U, P> const& q);

		GLM_FUNC_DECL GLM_EXPLICIT GLM_CONSTEXPR tdualquat(mat<2, 4, T, Q> const& holder_mat);
		GLM_FUNC_DECL GLM_EXPLICIT GLM_CONSTEXPR tdualquat(mat<3, 4, T, Q> const& aug_mat);

		

		GLM_FUNC_DECL tdualquat<T, Q> & operator=(tdualquat<T, Q> const& m) GLM_DEFAULT;

		template<typename U>
		GLM_FUNC_DECL tdualquat<T, Q> & operator=(tdualquat<U, Q> const& m);
		template<typename U>
		GLM_FUNC_DECL tdualquat<T, Q> & operator*=(U s);
		template<typename U>
		GLM_FUNC_DECL tdualquat<T, Q> & operator/=(U s);
	};

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> operator+(tdualquat<T, Q> const& q);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> operator-(tdualquat<T, Q> const& q);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> operator+(tdualquat<T, Q> const& q, tdualquat<T, Q> const& p);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> operator*(tdualquat<T, Q> const& q, tdualquat<T, Q> const& p);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> operator*(tdualquat<T, Q> const& q, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> operator*(vec<3, T, Q> const& v, tdualquat<T, Q> const& q);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> operator*(tdualquat<T, Q> const& q, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> operator*(vec<4, T, Q> const& v, tdualquat<T, Q> const& q);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> operator*(tdualquat<T, Q> const& q, T const& s);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> operator*(T const& s, tdualquat<T, Q> const& q);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> operator/(tdualquat<T, Q> const& q, T const& s);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL bool operator==(tdualquat<T, Q> const& q1, tdualquat<T, Q> const& q2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL bool operator!=(tdualquat<T, Q> const& q1, tdualquat<T, Q> const& q2);

	
	
	
	template <typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> dual_quat_identity();

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> normalize(tdualquat<T, Q> const& q);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> lerp(tdualquat<T, Q> const& x, tdualquat<T, Q> const& y, T const& a);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> inverse(tdualquat<T, Q> const& q);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<2, 4, T, Q> mat2x4_cast(tdualquat<T, Q> const& x);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 4, T, Q> mat3x4_cast(tdualquat<T, Q> const& x);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> dualquat_cast(mat<2, 4, T, Q> const& x);

	
	
	
	template<typename T, qualifier Q>
	GLM_FUNC_DECL tdualquat<T, Q> dualquat_cast(mat<3, 4, T, Q> const& x);


	
	
	
	typedef tdualquat<float, lowp>		lowp_dualquat;

	
	
	
	typedef tdualquat<float, mediump>	mediump_dualquat;

	
	
	
	typedef tdualquat<float, highp>		highp_dualquat;


	
	
	
	typedef tdualquat<float, lowp>		lowp_fdualquat;

	
	
	
	typedef tdualquat<float, mediump>	mediump_fdualquat;

	
	
	
	typedef tdualquat<float, highp>		highp_fdualquat;


	
	
	
	typedef tdualquat<double, lowp>		lowp_ddualquat;

	
	
	
	typedef tdualquat<double, mediump>	mediump_ddualquat;

	
	
	
	typedef tdualquat<double, highp>	highp_ddualquat;


#if(!defined(GLM_PRECISION_HIGHP_FLOAT) && !defined(GLM_PRECISION_MEDIUMP_FLOAT) && !defined(GLM_PRECISION_LOWP_FLOAT))
	
	
	
	typedef highp_fdualquat			dualquat;

	
	
	
	typedef highp_fdualquat			fdualquat;
#elif(defined(GLM_PRECISION_HIGHP_FLOAT) && !defined(GLM_PRECISION_MEDIUMP_FLOAT) && !defined(GLM_PRECISION_LOWP_FLOAT))
	typedef highp_fdualquat			dualquat;
	typedef highp_fdualquat			fdualquat;
#elif(!defined(GLM_PRECISION_HIGHP_FLOAT) && defined(GLM_PRECISION_MEDIUMP_FLOAT) && !defined(GLM_PRECISION_LOWP_FLOAT))
	typedef mediump_fdualquat		dualquat;
	typedef mediump_fdualquat		fdualquat;
#elif(!defined(GLM_PRECISION_HIGHP_FLOAT) && !defined(GLM_PRECISION_MEDIUMP_FLOAT) && defined(GLM_PRECISION_LOWP_FLOAT))
	typedef lowp_fdualquat			dualquat;
	typedef lowp_fdualquat			fdualquat;
#else
#	error "GLM error: multiple default precision requested for single-precision floating-point types"
#endif


#if(!defined(GLM_PRECISION_HIGHP_DOUBLE) && !defined(GLM_PRECISION_MEDIUMP_DOUBLE) && !defined(GLM_PRECISION_LOWP_DOUBLE))
	
	
	
	typedef highp_ddualquat			ddualquat;
#elif(defined(GLM_PRECISION_HIGHP_DOUBLE) && !defined(GLM_PRECISION_MEDIUMP_DOUBLE) && !defined(GLM_PRECISION_LOWP_DOUBLE))
	typedef highp_ddualquat			ddualquat;
#elif(!defined(GLM_PRECISION_HIGHP_DOUBLE) && defined(GLM_PRECISION_MEDIUMP_DOUBLE) && !defined(GLM_PRECISION_LOWP_DOUBLE))
	typedef mediump_ddualquat		ddualquat;
#elif(!defined(GLM_PRECISION_HIGHP_DOUBLE) && !defined(GLM_PRECISION_MEDIUMP_DOUBLE) && defined(GLM_PRECISION_LOWP_DOUBLE))
	typedef lowp_ddualquat			ddualquat;
#else
#	error "GLM error: Multiple default precision requested for double-precision floating-point types"
#endif

	
} 

#include "dual_quaternion.inl"
