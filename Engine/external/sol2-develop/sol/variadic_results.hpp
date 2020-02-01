






















#ifndef SOL_VARIADIC_RESULTS_HPP
#define SOL_VARIADIC_RESULTS_HPP

#include "stack.hpp"
#include "object.hpp"
#include "as_returns.hpp"
#include <vector>

namespace sol {

	struct variadic_results : public std::vector<object> {
		using std::vector<object>::vector;
	};

	namespace stack {
		template <>
		struct pusher<variadic_results> {
			int push(lua_State* L, const variadic_results& e) {
				int p = 0;
				for (const auto& i : e) {
					p += stack::push(L, i);
				}
				return p;
			}
		};
	} 

} 

#endif 
