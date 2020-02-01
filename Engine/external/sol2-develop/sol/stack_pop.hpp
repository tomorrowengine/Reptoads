






















#ifndef SOL_STACK_POP_HPP
#define SOL_STACK_POP_HPP

#include "stack_core.hpp"
#include "stack_get.hpp"
#include <utility>
#include <tuple>

namespace sol {
namespace stack {
	template <typename T, typename>
	struct popper {
		inline static decltype(auto) pop(lua_State* L) {
			record tracking{};
#ifdef __INTEL_COMPILER
			auto&& r = get<T>(L, -lua_size<T>::value, tracking);
#else
			decltype(auto) r = get<T>(L, -lua_size<T>::value, tracking);
#endif
			lua_pop(L, tracking.used);
			return r;
		}
	};

	template <typename T>
	struct popper<T, std::enable_if_t<is_stack_based<meta::unqualified_t<T>>::value>> {
		static_assert(meta::neg<is_stack_based<meta::unqualified_t<T>>>::value, "You cannot pop something that lives solely on the stack: it will not remain on the stack when popped and thusly will go out of scope!");
	};
}
} 

#endif 
