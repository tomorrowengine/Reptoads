






















#ifndef SOL_TUPLE_HPP
#define SOL_TUPLE_HPP

#include <tuple>
#include <cstddef>

namespace sol {
	namespace detail {
		using swallow = std::initializer_list<int>;
	} 

	template <typename... Args>
	struct types {
		typedef std::make_index_sequence<sizeof...(Args)> indices;
		static constexpr std::size_t size() {
			return sizeof...(Args);
		}
	};
	namespace meta {
		namespace detail {
			template <typename... Args>
			struct tuple_types_ { typedef types<Args...> type; };

			template <typename... Args>
			struct tuple_types_<std::tuple<Args...>> { typedef types<Args...> type; };
		} 

		template <typename T>
		using unqualified = std::remove_cv<std::remove_reference_t<T>>;

		template <typename T>
		using unqualified_t = typename unqualified<T>::type;

		template <typename... Args>
		using tuple_types = typename detail::tuple_types_<Args...>::type;

		template <typename Arg>
		struct pop_front_type;

		template <typename Arg>
		using pop_front_type_t = typename pop_front_type<Arg>::type;

		template <typename... Args>
		struct pop_front_type<types<Args...>> {
			typedef void front_type;
			typedef types<Args...> type;
		};

		template <typename Arg, typename... Args>
		struct pop_front_type<types<Arg, Args...>> {
			typedef Arg front_type;
			typedef types<Args...> type;
		};

		template <std::size_t N, typename Tuple>
		using tuple_element = std::tuple_element<N, std::remove_reference_t<Tuple>>;

		template <std::size_t N, typename Tuple>
		using tuple_element_t = std::tuple_element_t<N, std::remove_reference_t<Tuple>>;

		template <std::size_t N, typename Tuple>
		using unqualified_tuple_element = unqualified<tuple_element_t<N, Tuple>>;

		template <std::size_t N, typename Tuple>
		using unqualified_tuple_element_t = unqualified_t<tuple_element_t<N, Tuple>>;

	} 
} 

#endif 
