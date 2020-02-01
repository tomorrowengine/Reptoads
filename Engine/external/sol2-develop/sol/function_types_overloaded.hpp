






















#ifndef SOL_FUNCTION_TYPES_OVERLOAD_HPP
#define SOL_FUNCTION_TYPES_OVERLOAD_HPP

#include "overload.hpp"
#include "call.hpp"
#include "function_types_core.hpp"

namespace sol {
namespace function_detail {
	template <int start_skew = 0, typename... Functions>
	struct overloaded_function {
		typedef std::tuple<Functions...> overload_list;
		typedef std::make_index_sequence<sizeof...(Functions)> indices;
		overload_list overloads;

		overloaded_function(overload_list set)
		: overloads(std::move(set)) {
		}

		overloaded_function(Functions... fxs)
		: overloads(fxs...) {
		}

		template <typename Fx, std::size_t I, typename... R, typename... Args>
		int call(types<Fx>, index_value<I>, types<R...>, types<Args...>, lua_State* L, int, int) {
			auto& func = std::get<I>(overloads);
			return call_detail::call_wrapped<void, true, false, start_skew>(L, func);
		}

		int operator()(lua_State* L) {
			auto mfx = [&](auto&&... args) { return this->call(std::forward<decltype(args)>(args)...); };
			return call_detail::overload_match<Functions...>(mfx, L, 1 + start_skew);
		}
	};
}
} 

#endif 
