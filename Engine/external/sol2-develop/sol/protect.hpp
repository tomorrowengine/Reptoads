






















#ifndef SOL_PROTECT_HPP
#define SOL_PROTECT_HPP

#include "traits.hpp"
#include <utility>

namespace sol {

	template <typename T>
	struct protect_t {
		T value;

		template <typename Arg, typename... Args, meta::disable<std::is_same<protect_t, meta::unqualified_t<Arg>>> = meta::enabler>
		protect_t(Arg&& arg, Args&&... args)
		: value(std::forward<Arg>(arg), std::forward<Args>(args)...) {
		}

		protect_t(const protect_t&) = default;
		protect_t(protect_t&&) = default;
		protect_t& operator=(const protect_t&) = default;
		protect_t& operator=(protect_t&&) = default;
	};

	template <typename T>
	auto protect(T&& value) {
		return protect_t<std::decay_t<T>>(std::forward<T>(value));
	}

} 

#endif 
