


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
	struct vec<2, T, Q>
	{
		

		typedef T value_type;
		typedef vec<2, T, Q> type;
		typedef vec<2, bool, Q> bool_type;

		

#		if GLM_CONFIG_XYZW_ONLY
			T x, y;
#		elif GLM_CONFIG_ANONYMOUS_STRUCT == GLM_ENABLE
			union
			{
				struct{ T x, y; };
				struct{ T r, g; };
				struct{ T s, t; };

				typename detail::storage<2, T, detail::is_aligned<Q>::value>::type data;

#				if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
					GLM_SWIZZLE2_2_MEMBERS(T, Q, x, y)
					GLM_SWIZZLE2_2_MEMBERS(T, Q, r, g)
					GLM_SWIZZLE2_2_MEMBERS(T, Q, s, t)
					GLM_SWIZZLE2_3_MEMBERS(T, Q, x, y)
					GLM_SWIZZLE2_3_MEMBERS(T, Q, r, g)
					GLM_SWIZZLE2_3_MEMBERS(T, Q, s, t)
					GLM_SWIZZLE2_4_MEMBERS(T, Q, x, y)
					GLM_SWIZZLE2_4_MEMBERS(T, Q, r, g)
					GLM_SWIZZLE2_4_MEMBERS(T, Q, s, t)
#				endif
			};
#		else
			union {T x, r, s;};
			union {T y, g, t;};

#			if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
				GLM_SWIZZLE_GEN_VEC_FROM_VEC2(T, Q)
#			endif
#		endif

		

		
		typedef length_t length_type;
		GLM_FUNC_DECL static GLM_CONSTEXPR length_type length(){return 2;}

		GLM_FUNC_DECL GLM_CONSTEXPR T& operator[](length_type i);
		GLM_FUNC_DECL GLM_CONSTEXPR T const& operator[](length_type i) const;

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec() GLM_DEFAULT;
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec const& v) GLM_DEFAULT;
		template<qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, T, P> const& v);

		

		GLM_FUNC_DECL GLM_CONSTEXPR explicit vec(T scalar);
		GLM_FUNC_DECL GLM_CONSTEXPR vec(T x, T y);

		

		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR explicit vec(vec<1, U, P> const& v);

		
		template<typename A, typename B>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A x, B y);
		template<typename A, typename B>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, Q> const& x, B y);
		template<typename A, typename B>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A x, vec<1, B, Q> const& y);
		template<typename A, typename B>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, Q> const& x, vec<1, B, Q> const& y);

		

		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<3, U, P> const& v);
		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<4, U, P> const& v);

		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<2, U, P> const& v);

		
#		if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
			template<int E0, int E1>
			GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<2, T, Q, E0, E1,-1,-2> const& that)
			{
				*this = that();
			}
#		endif

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator=(vec const& v) GLM_DEFAULT;

		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator=(vec<2, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator+=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator+=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator+=(vec<2, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator-=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator-=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator-=(vec<2, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator*=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator*=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator*=(vec<2, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator/=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator/=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator/=(vec<2, U, Q> const& v);

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator++();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator--();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator++(int);
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator--(int);

		

		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator%=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator%=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator%=(vec<2, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator&=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator&=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator&=(vec<2, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator|=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator|=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator|=(vec<2, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator^=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator^=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator^=(vec<2, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator<<=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator<<=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator<<=(vec<2, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator>>=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator>>=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> & operator>>=(vec<2, U, Q> const& v);
	};

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator+(vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator-(vec<2, T, Q> const& v);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator+(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator+(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator+(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator+(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator+(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator-(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator-(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator-(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator-(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator-(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator*(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator*(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator*(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator*(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator*(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator/(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator/(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator/(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator/(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator/(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator%(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator%(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator%(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator%(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator%(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator&(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator&(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator&(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator&(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator&(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator|(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator|(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator|(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator|(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator|(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator^(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator^(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator^(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator^(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator^(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator<<(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator<<(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator<<(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator<<(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator<<(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator>>(vec<2, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator>>(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator>>(T scalar, vec<2, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator>>(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator>>(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> operator~(vec<2, T, Q> const& v);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator==(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator!=(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2);

	template<qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, bool, Q> operator&&(vec<2, bool, Q> const& v1, vec<2, bool, Q> const& v2);

	template<qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<2, bool, Q> operator||(vec<2, bool, Q> const& v1, vec<2, bool, Q> const& v2);
}

#ifndef GLM_EXTERNAL_TEMPLATE
#include "type_vec2.inl"
#endif
