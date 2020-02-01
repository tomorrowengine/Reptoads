






















#ifndef SOL_IN_PLACE_HPP
#define SOL_IN_PLACE_HPP

#include <cstddef>
#include <utility>

namespace sol {

#if defined(SOL_CXX17_FEATURES) && SOL_CXX17_FEATURES
	using in_place_t = std::in_place_t;
	constexpr std::in_place_t in_place{};
	constexpr std::in_place_t in_place_of{};

	template <typename T>
	using in_place_type_t = std::in_place_type_t<T>;
	template <typename T>
	constexpr std::in_place_type_t<T> in_place_type{};

	template <size_t I>
	using in_place_index_t = std::in_place_index_t<I>;
	template <size_t I>
	constexpr in_place_index_t<I> in_place_index{};
#else
	namespace detail {
		struct in_place_of_tag {};
		template <std::size_t I>
		struct in_place_of_i {};
		template <typename T>
		struct in_place_of_t {};
	} 

	struct in_place_tag {
		constexpr in_place_tag() = default;
	};

	constexpr inline in_place_tag in_place(detail::in_place_of_tag) {
		return in_place_tag();
	}
	template <typename T>
	constexpr inline in_place_tag in_place(detail::in_place_of_t<T>) {
		return in_place_tag();
	}
	template <std::size_t I>
	constexpr inline in_place_tag in_place(detail::in_place_of_i<I>) {
		return in_place_tag();
	}

	constexpr inline in_place_tag in_place_of(detail::in_place_of_tag) {
		return in_place_tag();
	}
	template <typename T>
	constexpr inline in_place_tag in_place_type(detail::in_place_of_t<T>) {
		return in_place_tag();
	}
	template <std::size_t I>
	constexpr inline in_place_tag in_place_index(detail::in_place_of_i<I>) {
		return in_place_tag();
	}

	using in_place_t = in_place_tag (&)(detail::in_place_of_tag);
	template <typename T>
	using in_place_type_t = in_place_tag (&)(detail::in_place_of_t<T>);
	template <std::size_t I>
	using in_place_index_t = in_place_tag (&)(detail::in_place_of_i<I>);
#endif

} 

#endif 
