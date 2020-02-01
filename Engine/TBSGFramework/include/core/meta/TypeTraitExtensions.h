#pragma once
#include <type_traits>

namespace meta
{
	namespace detail {
		template <typename T>
		struct is_complete_helper {
			template <typename U>
			static auto test(U*)->std::integral_constant<bool, sizeof(U) == sizeof(U)>;
			static auto test(...)->std::false_type;
			using type = decltype(test((T*)0));
		};
	}

	template <typename T>
	struct is_complete_t : detail::is_complete_helper<T>::type {};
}