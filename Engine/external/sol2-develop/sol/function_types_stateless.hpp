






















#ifndef SOL_FUNCTION_TYPES_STATELESS_HPP
#define SOL_FUNCTION_TYPES_STATELESS_HPP

#include "stack.hpp"

namespace sol {
namespace function_detail {
	template <typename Function, bool is_yielding>
	struct upvalue_free_function {
		typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
		typedef meta::bind_traits<function_type> traits_type;

		static int real_call(lua_State* L) noexcept(traits_type::is_noexcept) {
			auto udata = stack::stack_detail::get_as_upvalues<function_type*>(L);
			function_type* fx = udata.first;
			return call_detail::call_wrapped<void, true, false>(L, fx);
		}

		static int call(lua_State* L) {
			int nr = detail::typed_static_trampoline<decltype(&real_call), (&real_call)>(L);
			if (is_yielding) {
				return lua_yield(L, nr);
			}
			else {
				return nr;
			}
		}

		int operator()(lua_State* L) {
			return call(L);
		}
	};

	template <typename T, typename Function, bool is_yielding>
	struct upvalue_member_function {
		typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
		typedef lua_bind_traits<function_type> traits_type;

		static int real_call(lua_State* L) noexcept(traits_type::is_noexcept) {
			
			
			
			
			
			auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L);
			auto objdata = stack::stack_detail::get_as_upvalues<T*>(L, memberdata.second);
			function_type& memfx = memberdata.first;
			auto& item = *objdata.first;
			return call_detail::call_wrapped<T, true, false, -1>(L, memfx, item);
		}

		static int call(lua_State* L) {
			int nr = detail::typed_static_trampoline<decltype(&real_call), (&real_call)>(L);
			if (is_yielding) {
				return lua_yield(L, nr);
			}
			else {
				return nr;
			}
		}

		int operator()(lua_State* L) {
			return call(L);
		}
	};

	template <typename T, typename Function, bool is_yielding>
	struct upvalue_member_variable {
		typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
		typedef lua_bind_traits<function_type> traits_type;

		static int real_call(lua_State* L) noexcept(traits_type::is_noexcept) {
			
			
			
			
			
			auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L);
			auto objdata = stack::stack_detail::get_as_upvalues<T*>(L, memberdata.second);
			auto& mem = *objdata.first;
			function_type& var = memberdata.first;
			switch (lua_gettop(L)) {
			case 0:
				return call_detail::call_wrapped<T, true, false, -1>(L, var, mem);
			case 1:
				return call_detail::call_wrapped<T, false, false, -1>(L, var, mem);
			default:
				return luaL_error(L, "sol: incorrect number of arguments to member variable function");
			}
		}

		static int call(lua_State* L) {
			int nr = detail::typed_static_trampoline<decltype(&real_call), (&real_call)>(L);
			if (is_yielding) {
				return lua_yield(L, nr);
			}
			else {
				return nr;
			}
		}

		int operator()(lua_State* L) {
			return call(L);
		}
	};

	template <typename T, typename Function, bool is_yielding>
	struct upvalue_member_variable<T, readonly_wrapper<Function>, is_yielding> {
		typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
		typedef lua_bind_traits<function_type> traits_type;

		static int real_call(lua_State* L) noexcept(traits_type::is_noexcept) {
			
			
			
			
			
			auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L);
			auto objdata = stack::stack_detail::get_as_upvalues<T*>(L, memberdata.second);
			auto& mem = *objdata.first;
			function_type& var = memberdata.first;
			switch (lua_gettop(L)) {
			case 0:
				return call_detail::call_wrapped<T, true, false, -1>(L, var, mem);
			default:
				return luaL_error(L, "sol: incorrect number of arguments to member variable function");
			}
		}

		static int call(lua_State* L) {
			int nr = detail::typed_static_trampoline<decltype(&real_call), (&real_call)>(L);
			if (is_yielding) {
				return lua_yield(L, nr);
			}
			else {
				return nr;
			}
		}

		int operator()(lua_State* L) {
			return call(L);
		}
	};

	template <typename T, typename Function, bool is_yielding>
	struct upvalue_this_member_function {
		typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
		typedef lua_bind_traits<function_type> traits_type;

		static int real_call(lua_State* L) noexcept(traits_type::is_noexcept) {
			
			
			auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L);
			function_type& memfx = memberdata.first;
			return call_detail::call_wrapped<T, false, false>(L, memfx);
		}

		static int call(lua_State* L) {
			int nr = detail::typed_static_trampoline<decltype(&real_call), (&real_call)>(L);
			if (is_yielding) {
				return lua_yield(L, nr);
			}
			else {
				return nr;
			}
		}

		int operator()(lua_State* L) {
			return call(L);
		}
	};

	template <typename T, typename Function, bool is_yielding>
	struct upvalue_this_member_variable {
		typedef std::remove_pointer_t<std::decay_t<Function>> function_type;

		static int real_call(lua_State* L) noexcept(false) {
			
			
			auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L);
			function_type& var = memberdata.first;
			switch (lua_gettop(L)) {
			case 1:
				return call_detail::call_wrapped<T, true, false>(L, var);
			case 2:
				return call_detail::call_wrapped<T, false, false>(L, var);
			default:
				return luaL_error(L, "sol: incorrect number of arguments to member variable function");
			}
		}

		static int call(lua_State* L) {
			int nr = detail::typed_static_trampoline<decltype(&real_call), (&real_call)>(L);
			if (is_yielding) {
				return lua_yield(L, nr);
			}
			else {
				return nr;
			}
		}

		int operator()(lua_State* L) {
			return call(L);
		}
	};

	template <typename T, typename Function, bool is_yielding>
	struct upvalue_this_member_variable<T, readonly_wrapper<Function>, is_yielding> {
		typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
		typedef lua_bind_traits<function_type> traits_type;

		static int real_call(lua_State* L) noexcept(false) {
			
			
			auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L);
			function_type& var = memberdata.first;
			switch (lua_gettop(L)) {
			case 1:
				return call_detail::call_wrapped<T, true, false>(L, var);
			default:
				return luaL_error(L, "sol: incorrect number of arguments to member variable function");
			}
		}

		static int call(lua_State* L) {
			int nr = detail::typed_static_trampoline<decltype(&real_call), (&real_call)>(L);
			if (is_yielding) {
				return lua_yield(L, nr);
			}
			else {
				return nr;
			}
		}

		int operator()(lua_State* L) {
			return call(L);
		}
	};
}
} 

#endif 
