






















#ifndef SOL_FUNCTION_TYPES_CORE_HPP
#define SOL_FUNCTION_TYPES_CORE_HPP

#include "stack.hpp"
#include "wrapper.hpp"
#include <memory>

namespace sol {
namespace function_detail {
	template <typename Fx, int start = 1, bool is_yielding = false>
	inline int call(lua_State* L) {
		Fx& fx = stack::get<user<Fx>>(L, upvalue_index(start));
		int nr = fx(L);
		if (is_yielding) {
			return lua_yield(L, nr);
		}
		else {
			return nr;
		}
	}
}
} 

#endif 
