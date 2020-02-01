


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
	struct vec<4, T, Q>
	{
		

		typedef T value_type;
		typedef vec<4, T, Q> type;
		typedef vec<4, bool, Q> bool_type;

		

#		if GLM_CONFIG_XYZW_ONLY
			T x, y, z, w;
#		elif GLM_CONFIG_ANONYMOUS_STRUCT == GLM_ENABLE
			union
			{
				struct { T x, y, z, w; };
				struct { T r, g, b, a; };
				struct { T s, t, p, q; };

				typename detail::storage<4, T, detail::is_aligned<Q>::value>::type data;

#				if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
					GLM_SWIZZLE4_2_MEMBERS(T, Q, x, y, z, w)
					GLM_SWIZZLE4_2_MEMBERS(T, Q, r, g, b, a)
					GLM_SWIZZLE4_2_MEMBERS(T, Q, s, t, p, q)
					GLM_SWIZZLE4_3_MEMBERS(T, Q, x, y, z, w)
					GLM_SWIZZLE4_3_MEMBERS(T, Q, r, g, b, a)
					GLM_SWIZZLE4_3_MEMBERS(T, Q, s, t, p, q)
					GLM_SWIZZLE4_4_MEMBERS(T, Q, x, y, z, w)
					GLM_SWIZZLE4_4_MEMBERS(T, Q, r, g, b, a)
					GLM_SWIZZLE4_4_MEMBERS(T, Q, s, t, p, q)
#				endif
			};
#		else
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };
			union { T w, a, q; };

#			if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
				GLM_SWIZZLE_GEN_VEC_FROM_VEC4(T, Q)
#			endif
#		endif

		

		
		typedef length_t length_type;
		GLM_FUNC_DECL static GLM_CONSTEXPR length_type length(){return 4;}

		GLM_FUNC_DECL GLM_CONSTEXPR T & operator[](length_type i);
		GLM_FUNC_DECL GLM_CONSTEXPR T const& operator[](length_type i) const;

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec() GLM_DEFAULT;
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<4, T, Q> const& v) GLM_DEFAULT;
		template<qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<4, T, P> const& v);

		

		GLM_FUNC_DECL GLM_CONSTEXPR explicit vec(T scalar);
		GLM_FUNC_DECL GLM_CONSTEXPR vec(T x, T y, T z, T w);

		

		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR explicit vec(vec<1, U, P> const& v);

		
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, Y _y, Z _z, W _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, Y _y, Z _z, W _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const& _y, Z _z, W _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, vec<1, Y, Q> const& _y, Z _z, W _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, Y _y, vec<1, Z, Q> const& _z, W _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, Y _y, vec<1, Z, Q> const& _z, W _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const& _y, vec<1, Z, Q> const& _z, W _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, vec<1, Y, Q> const& _y, vec<1, Z, Q> const& _z, W _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, Y _y, Z _z, vec<1, W, Q> const& _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const& _y, Z _z, vec<1, W, Q> const& _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, vec<1, Y, Q> const& _y, Z _z, vec<1, W, Q> const& _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, Y _y, vec<1, Z, Q> const& _z, vec<1, W, Q> const& _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, Y _y, vec<1, Z, Q> const& _z, vec<1, W, Q> const& _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const& _y, vec<1, Z, Q> const& _z, vec<1, W, Q> const& _w);
		template<typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const& _x, vec<1, Y, Q> const& _Y, vec<1, Z, Q> const& _z, vec<1, W, Q> const& _w);

		

		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const& _xy, B _z, C _w);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const& _xy, vec<1, B, P> const& _z, C _w);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const& _xy, B _z, vec<1, C, P> const& _w);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const& _xy, vec<1, B, P> const& _z, vec<1, C, P> const& _w);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, vec<2, B, P> const& _yz, C _w);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const& _x, vec<2, B, P> const& _yz, C _w);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, vec<2, B, P> const& _yz, vec<1, C, P> const& _w);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const& _x, vec<2, B, P> const& _yz, vec<1, C, P> const& _w);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, B _y, vec<2, C, P> const& _zw);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const& _x, B _y, vec<2, C, P> const& _zw);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, vec<1, B, P> const& _y, vec<2, C, P> const& _zw);
		
		template<typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const& _x, vec<1, B, P> const& _y, vec<2, C, P> const& _zw);
		
		template<typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<3, A, P> const& _xyz, B _w);
		
		template<typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<3, A, P> const& _xyz, vec<1, B, P> const& _w);
		
		template<typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, vec<3, B, P> const& _yzw);
		
		template<typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const& _x, vec<3, B, P> const& _yzw);
		
		template<typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const& _xy, vec<2, B, P> const& _zw);

		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<4, U, P> const& v);

		
#		if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
			template<int E0, int E1, int E2, int E3>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<4, T, Q, E0, E1, E2, E3> const& that)
			{
				*this = that();
			}

			template<int E0, int E1, int F0, int F1>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<2, T, Q, E0, E1, -1, -2> const& v, detail::_swizzle<2, T, Q, F0, F1, -1, -2> const& u)
			{
				*this = vec<4, T, Q>(v(), u());
			}

			template<int E0, int E1>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(T const& x, T const& y, detail::_swizzle<2, T, Q, E0, E1, -1, -2> const& v)
			{
				*this = vec<4, T, Q>(x, y, v());
			}

			template<int E0, int E1>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(T const& x, detail::_swizzle<2, T, Q, E0, E1, -1, -2> const& v, T const& w)
			{
				*this = vec<4, T, Q>(x, v(), w);
			}

			template<int E0, int E1>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<2, T, Q, E0, E1, -1, -2> const& v, T const& z, T const& w)
			{
				*this = vec<4, T, Q>(v(), z, w);
			}

			template<int E0, int E1, int E2>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<3, T, Q, E0, E1, E2, -1> const& v, T const& w)
			{
				*this = vec<4, T, Q>(v(), w);
			}

			template<int E0, int E1, int E2>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(T const& x, detail::_swizzle<3, T, Q, E0, E1, E2, -1> const& v)
			{
				*this = vec<4, T, Q>(x, v());
			}
#		endif

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator=(vec<4, T, Q> const& v) GLM_DEFAULT;

		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator=(vec<4, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator+=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator+=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator+=(vec<4, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator-=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator-=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator-=(vec<4, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator*=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator*=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator*=(vec<4, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator/=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator/=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q>& operator/=(vec<4, U, Q> const& v);

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator++();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator--();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator++(int);
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator--(int);

		

		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator%=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator%=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator%=(vec<4, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator&=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator&=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator&=(vec<4, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator|=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator|=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator|=(vec<4, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator^=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator^=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator^=(vec<4, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator<<=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator<<=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator<<=(vec<4, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator>>=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator>>=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> & operator>>=(vec<4, U, Q> const& v);
	};

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const& v);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const& v, T const & scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<1, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const& v, T const & scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<1, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(vec<4, T, Q> const& v, T const & scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(vec<4, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(vec<1, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(vec<4, T, Q> const& v, T const & scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(vec<4, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(vec<1, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(vec<4, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(vec<4, T, Q> const& v, vec<1, T, Q> const& scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(vec<1, T, Q> const& scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(vec<4, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(vec<4, T, Q> const& v, vec<1, T, Q> const& scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(vec<1, T, Q> const& scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(vec<4, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(vec<4, T, Q> const& v, vec<1, T, Q> const& scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(vec<1, T, Q> const& scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(vec<4, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(vec<4, T, Q> const& v, vec<1, T, Q> const& scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(vec<1, T, Q> const& scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<4, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<4, T, Q> const& v, vec<1, T, Q> const& scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<1, T, Q> const& scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<4, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<4, T, Q> const& v, vec<1, T, Q> const& scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(T scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<1, T, Q> const& scalar, vec<4, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator~(vec<4, T, Q> const& v);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator==(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator!=(vec<4, T, Q> const& v1, vec<4, T, Q> const& v2);

	template<qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, bool, Q> operator&&(vec<4, bool, Q> const& v1, vec<4, bool, Q> const& v2);

	template<qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, bool, Q> operator||(vec<4, bool, Q> const& v1, vec<4, bool, Q> const& v2);
}

#ifndef GLM_EXTERNAL_TEMPLATE
#include "type_vec4.inl"
#endif
