


#pragma once

#include "qualifier.hpp"
#if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
#	include "_swizzle.hpp"
#elif GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
#	include "_swizzle_func.hpp"
#endif
#include <cstddef>

namespace glm
{
	template<typename T, qualifier Q>
	struct vec<3, T, Q>
	{
		

		typedef T value_type;
		typedef vec<3, T, Q> type;
		typedef vec<3, bool, Q> bool_type;

		

#		if GLM_CONFIG_XYZW_ONLY
			T x, y, z;
#		elif GLM_CONFIG_ANONYMOUS_STRUCT == GLM_ENABLE
			union
			{
				struct{ T x, y, z; };
				struct{ T r, g, b; };
				struct{ T s, t, p; };

				typename detail::storage<3, T, detail::is_aligned<Q>::value>::type data;

#				if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
					GLM_SWIZZLE3_2_MEMBERS(T, Q, x, y, z)
					GLM_SWIZZLE3_2_MEMBERS(T, Q, r, g, b)
					GLM_SWIZZLE3_2_MEMBERS(T, Q, s, t, p)
					GLM_SWIZZLE3_3_MEMBERS(T, Q, x, y, z)
					GLM_SWIZZLE3_3_MEMBERS(T, Q, r, g, b)
					GLM_SWIZZLE3_3_MEMBERS(T, Q, s, t, p)
					GLM_SWIZZLE3_4_MEMBERS(T, Q, x, y, z)
					GLM_SWIZZLE3_4_MEMBERS(T, Q, r, g, b)
					GLM_SWIZZLE3_4_MEMBERS(T, Q, s, t, p)
#				endif
			};
#		else
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };

#			if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
				GLM_SWIZZLE_GEN_VEC_FROM_VEC3(T, Q)
#			endif
#		endif

		

		
		typedef length_t length_type;
		GLM_FUNC_DECL static GLM_CONSTEXPR length_type length(){return 3;}

		GLM_FUNC_DECL GLM_CONSTEXPR T & operator[](length_type i);
		GLM_FUNC_DECL GLM_CONSTEXPR T const& operator[](length_type i) const;

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec() GLM_DEFAULT;
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec const& v) GLM_DEFAULT;
		template<qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<3, T, P> const& v);

		

		GLM_FUNC_DECL GLM_CONSTEXPR explicit vec(T scalar);
		GLM_FUNC_DECL GLM_CONSTEXPR vec(T a, T b, T c);

		

		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR explicit vec(vec<1, U, P> const& v);

		
		template<typename X, typename Y, typename Z>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X x, Y y, Z z);
		template<typename X, typename Y, typename Z>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, Y _y, Z _z);
		template<typename X, typename Y, typename Z>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const& _y, Z _z);
		template<typename X, typename Y, typename Z>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, vec<1, Y, Q> const& _y, Z _z);
		template<typename X, typename Y, typename Z>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, Y _y, vec<1, Z, Q> const& _z);
		template<typename X, typename Y, typename Z>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, Y _y, vec<1, Z, Q> const& _z);
		template<typename X, typename Y, typename Z>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const& _y, vec<1, Z, Q> const& _z);
		template<typename X, typename Y, typename Z>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, vec<1, Y, Q> const& _y, vec<1, Z, Q> const& _z);

		

		
		template<typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const& _xy, B _z);
		
		template<typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const& _xy, vec<1, B, P> const& _z);
		
		template<typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, vec<2, B, P> const& _yz);
		
		template<typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const& _x, vec<2, B, P> const& _yz);
		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<4, U, P> const& v);

		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<3, U, P> const& v);

		
#		if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
			template<int E0, int E1, int E2>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<3, T, Q, E0, E1, E2, -1> const& that)
			{
				*this = that();
			}

			template<int E0, int E1>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<2, T, Q, E0, E1, -1, -2> const& v, T const& scalar)
			{
				*this = vec(v(), scalar);
			}

			template<int E0, int E1>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(T const& scalar, detail::_swizzle<2, T, Q, E0, E1, -1, -2> const& v)
			{
				*this = vec(scalar, v());
			}
#		endif

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q>& operator=(vec<3, T, Q> const& v) GLM_DEFAULT;

		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator=(vec<3, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator+=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator+=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator+=(vec<3, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator-=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator-=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator-=(vec<3, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator*=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator*=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator*=(vec<3, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator/=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator/=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator/=(vec<3, U, Q> const& v);

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator++();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator--();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator++(int);
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator--(int);

		

		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator%=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator%=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator%=(vec<3, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator&=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator&=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator&=(vec<3, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator|=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator|=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator|=(vec<3, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator^=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator^=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator^=(vec<3, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator<<=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator<<=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator<<=(vec<3, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator>>=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator>>=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> & operator>>=(vec<3, U, Q> const& v);
	};

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator+(vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator-(vec<3, T, Q> const& v);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator+(vec<3, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator+(vec<3, T, Q> const& v, vec<1, T, Q> const& scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator+(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator+(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator+(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator-(vec<3, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator-(vec<3, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator-(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator-(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator-(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator*(vec<3, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator*(vec<3, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator*(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator*(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator*(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator/(vec<3, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator/(vec<3, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator/(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator/(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator/(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator%(vec<3, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator%(vec<3, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator%(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator%(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator%(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator&(vec<3, T, Q> const& v1, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator&(vec<3, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator&(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator&(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator&(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator|(vec<3, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator|(vec<3, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator|(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator|(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator|(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator^(vec<3, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator^(vec<3, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator^(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator^(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator^(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator<<(vec<3, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator<<(vec<3, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator<<(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator<<(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator<<(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator>>(vec<3, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator>>(vec<3, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator>>(T scalar, vec<3, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator>>(vec<1, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator>>(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> operator~(vec<3, T, Q> const& v);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator==(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator!=(vec<3, T, Q> const& v1, vec<3, T, Q> const& v2);

	template<qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, bool, Q> operator&&(vec<3, bool, Q> const& v1, vec<3, bool, Q> const& v2);

	template<qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<3, bool, Q> operator||(vec<3, bool, Q> const& v1, vec<3, bool, Q> const& v2);
}

#ifndef GLM_EXTERNAL_TEMPLATE
#include "type_vec3.inl"
#endif
