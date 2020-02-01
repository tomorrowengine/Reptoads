






















#ifndef SOL_AS_RETURNS_HPP
#define SOL_AS_RETURNS_HPP

#include "traits.hpp"
#include "stack.hpp"

namespace sol {
	template <typename T>
	struct as_returns_t {
		T src;
	};

	template <typename Source>
	auto as_returns(Source&& source) {
		return as_returns_t<std::decay_t<Source>>{ std::forward<Source>(source) };
	}

	namespace stack {
		template <typename T>
		struct pusher<as_returns_t<T>> {
			int push(lua_State* L, const as_returns_t<T>& e) {
				auto& src = detail::unwrap(e.src);
				int p = 0;
				for (const auto& i : src) {
					p += stack::push(L, i);
				}
				return p;
			}
		};
	} 
} 

#endif 
