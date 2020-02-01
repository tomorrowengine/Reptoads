






















#ifndef SOL_AS_ARGS_HPP
#define SOL_AS_ARGS_HPP

#include "stack.hpp"

namespace sol {
	template <typename T>
	struct as_args_t {
		T src;
	};

	template <typename Source>
	auto as_args(Source&& source) {
		return as_args_t<Source>{ std::forward<Source>(source) };
	}

	namespace stack {
		template <typename T>
		struct pusher<as_args_t<T>> {
			int push(lua_State* L, const as_args_t<T>& e) {
				int p = 0;
				for (const auto& i : e.src) {
					p += stack::push(L, i);
				}
				return p;
			}
		};
	} 
} 

#endif 
