






















#ifndef SOL_FUNCTION_HPP
#define SOL_FUNCTION_HPP

#include "stack.hpp"
#include "unsafe_function.hpp"
#include "protected_function.hpp"
#include <functional>

namespace sol {
	template <typename... Ret, typename... Args>
	inline decltype(auto) stack_proxy::call(Args&&... args) {
		stack_function sf(this->lua_state(), this->stack_index());
		return sf.template call<Ret...>(std::forward<Args>(args)...);
	}

	inline protected_function_result::protected_function_result(unsafe_function_result&& o) noexcept
	: L(o.lua_state()), index(o.stack_index()), returncount(o.return_count()), popcount(o.return_count()), err(o.status()) {
		
		
		
		o.abandon();
	}

	inline protected_function_result& protected_function_result::operator=(unsafe_function_result&& o) noexcept {
		L = o.lua_state();
		index = o.stack_index();
		returncount = o.return_count();
		popcount = o.return_count();
		err = o.status();
		
		
		
		o.abandon();
		return *this;
	}

	inline unsafe_function_result::unsafe_function_result(protected_function_result&& o) noexcept
	: L(o.lua_state()), index(o.stack_index()), returncount(o.return_count()) {
		
		
		
		o.abandon();
	}
	inline unsafe_function_result& unsafe_function_result::operator=(protected_function_result&& o) noexcept {
		L = o.lua_state();
		index = o.stack_index();
		returncount = o.return_count();
		
		
		
		o.abandon();
		return *this;
	}

	namespace stack {
		template <typename Signature>
		struct getter<std::function<Signature>> {
			typedef meta::bind_traits<Signature> fx_t;
			typedef typename fx_t::args_list args_lists;
			typedef meta::tuple_types<typename fx_t::return_type> return_types;

			template <typename... Args, typename... Ret>
			static std::function<Signature> get_std_func(types<Ret...>, types<Args...>, lua_State* L, int index) {
				unsafe_function f(L, index);
				auto fx = [ f = std::move(f) ](Args && ... args) -> meta::return_type_t<Ret...> {
					return f.call<Ret...>(std::forward<Args>(args)...);
				};
				return std::move(fx);
			}

			template <typename... FxArgs>
			static std::function<Signature> get_std_func(types<void>, types<FxArgs...>, lua_State* L, int index) {
				unsafe_function f(L, index);
				auto fx = [f = std::move(f)](FxArgs&&... args) -> void {
					f(std::forward<FxArgs>(args)...);
				};
				return std::move(fx);
			}

			template <typename... FxArgs>
			static std::function<Signature> get_std_func(types<>, types<FxArgs...> t, lua_State* L, int index) {
				return get_std_func(types<void>(), t, L, index);
			}

			static std::function<Signature> get(lua_State* L, int index, record& tracking) {
				tracking.last = 1;
				tracking.used += 1;
				type t = type_of(L, index);
				if (t == type::none || t == type::lua_nil) {
					return nullptr;
				}
				return get_std_func(return_types(), args_lists(), L, index);
			}
		};
	} 

} 

#endif 
