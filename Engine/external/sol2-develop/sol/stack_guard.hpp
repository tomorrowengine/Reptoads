






















#ifndef SOL_STACK_GUARD_HPP
#define SOL_STACK_GUARD_HPP

#include "compatibility/version.hpp"
#include "error.hpp"
#include <functional>

namespace sol {
	namespace detail {
		inline void stack_fail(int, int) {
#if !(defined(SOL_NO_EXCEPTIONS) && SOL_NO_EXCEPTIONS)
			throw error(detail::direct_error, "imbalanced stack after operation finish");
#else
			
			
			
#endif 
		}
	} 

	struct stack_guard {
		lua_State* L;
		int top;
		std::function<void(int, int)> on_mismatch;

		stack_guard(lua_State* L)
		: stack_guard(L, lua_gettop(L)) {
		}
		stack_guard(lua_State* L, int top, std::function<void(int, int)> fx = detail::stack_fail)
		: L(L), top(top), on_mismatch(std::move(fx)) {
		}
		bool check_stack(int modification = 0) const {
			int bottom = lua_gettop(L) + modification;
			if (top == bottom) {
				return true;
			}
			on_mismatch(top, bottom);
			return false;
		}
		~stack_guard() {
			check_stack();
		}
	};
} 

#endif 
