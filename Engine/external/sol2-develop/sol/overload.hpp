






















#ifndef SOL_OVERLOAD_HPP
#define SOL_OVERLOAD_HPP

#include "traits.hpp"
#include <utility>

namespace sol {
	template <typename... Functions>
	struct overload_set {
		std::tuple<Functions...> functions;
		template <typename Arg, typename... Args, meta::disable<std::is_same<overload_set, meta::unqualified_t<Arg>>> = meta::enabler>
		overload_set(Arg&& arg, Args&&... args)
		: functions(std::forward<Arg>(arg), std::forward<Args>(args)...) {
		}
		overload_set(const overload_set&) = default;
		overload_set(overload_set&&) = default;
		overload_set& operator=(const overload_set&) = default;
		overload_set& operator=(overload_set&&) = default;
	};

	template <typename... Args>
	decltype(auto) overload(Args&&... args) {
		return overload_set<std::decay_t<Args>...>(std::forward<Args>(args)...);
	}
} 

#endif 
