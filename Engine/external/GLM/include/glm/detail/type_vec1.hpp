


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
	struct vec<1, T, Q>
	{
		

		typedef T value_type;
		typedef vec<1, T, Q> type;
		typedef vec<1, bool, Q> bool_type;

		

#		if GLM_CONFIG_XYZW_ONLY
			T x;
#		elif GLM_CONFIG_ANONYMOUS_STRUCT == GLM_ENABLE
			union
			{
				T x;
				T r;
				T s;

				typename detail::storage<1, T, detail::is_aligned<Q>::value>::type data;

			};
#		else
			union {T x, r, s;};

#		endif

		

		
		typedef length_t length_type;
		GLM_FUNC_DECL static GLM_CONSTEXPR length_type length(){return 1;}

		GLM_FUNC_DECL GLM_CONSTEXPR T & operator[](length_type i);
		GLM_FUNC_DECL GLM_CONSTEXPR T const& operator[](length_type i) const;

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec() GLM_DEFAULT;
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec const& v) GLM_DEFAULT;
		template<qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, T, P> const& v);

		

		GLM_FUNC_DECL GLM_CONSTEXPR explicit vec(T scalar);

		

		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<2, U, P> const& v);
		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<3, U, P> const& v);
		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<4, U, P> const& v);

		
		template<typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<1, U, P> const& v);

		

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator=(vec const& v) GLM_DEFAULT;

		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator+=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator+=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator-=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator-=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator*=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator*=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator/=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator/=(vec<1, U, Q> const& v);

		

		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator++();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator--();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator++(int);
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator--(int);

		

		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator%=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator%=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator&=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator&=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator|=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator|=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator^=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator^=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator<<=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator<<=(vec<1, U, Q> const& v);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator>>=(U scalar);
		template<typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> & operator>>=(vec<1, U, Q> const& v);
	};

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator+(vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator-(vec<1, T, Q> const& v);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator+(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator+(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator+(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator-(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator-(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator-(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator*(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator*(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator*(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator/(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator/(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator/(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator%(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator%(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator%(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator&(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator&(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator&(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator|(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator|(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator|(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator^(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator^(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator^(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator<<(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator<<(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator<<(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator>>(vec<1, T, Q> const& v, T scalar);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator>>(T scalar, vec<1, T, Q> const& v);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator>>(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, T, Q> operator~(vec<1, T, Q> const& v);

	

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator==(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator!=(vec<1, T, Q> const& v1, vec<1, T, Q> const& v2);

	template<qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, bool, Q> operator&&(vec<1, bool, Q> const& v1, vec<1, bool, Q> const& v2);

	template<qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<1, bool, Q> operator||(vec<1, bool, Q> const& v1, vec<1, bool, Q> const& v2);
}

#ifndef GLM_EXTERNAL_TEMPLATE
#include "type_vec1.inl"
#endif
