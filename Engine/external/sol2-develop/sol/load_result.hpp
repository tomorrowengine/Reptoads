






















#ifndef SOL_LOAD_RESULT_HPP
#define SOL_LOAD_RESULT_HPP

#include "stack.hpp"
#include "function.hpp"
#include "proxy_base.hpp"
#include <cstdint>

namespace sol {
	struct load_result : public proxy_base<load_result> {
	private:
		lua_State* L;
		int index;
		int returncount;
		int popcount;
		load_status err;

		template <typename T>
		decltype(auto) tagged_get(types<optional<T>>) const {
			if (!valid()) {
				return optional<T>(nullopt);
			}
			return stack::get<optional<T>>(L, index);
		}

		template <typename T>
		decltype(auto) tagged_get(types<T>) const {
#if defined(SOL_SAFE_PROXIES) && SOL_SAFE_PROXIES != 0
			if (!valid()) {
				type_panic_c_str(L, index, type_of(L, index), type::none);
			}
#endif 
			return stack::get<T>(L, index);
		}

		optional<error> tagged_get(types<optional<error>>) const {
			if (valid()) {
				return nullopt;
			}
			return error(detail::direct_error, stack::get<std::string>(L, index));
		}

		error tagged_get(types<error>) const {
#if defined(SOL_SAFE_PROXIES) && SOL_SAFE_PROXIES != 0
			if (valid()) {
				type_panic_c_str(L, index, type_of(L, index), type::none, "expecting an error type (a string, from Lua)");
			}
#endif 
			return error(detail::direct_error, stack::get<std::string>(L, index));
		}

	public:
		load_result() = default;
		load_result(lua_State* Ls, int stackindex = -1, int retnum = 0, int popnum = 0, load_status lerr = load_status::ok) noexcept
		: L(Ls), index(stackindex), returncount(retnum), popcount(popnum), err(lerr) {
		}
		load_result(const load_result&) = default;
		load_result& operator=(const load_result&) = default;
		load_result(load_result&& o) noexcept
		: L(o.L), index(o.index), returncount(o.returncount), popcount(o.popcount), err(o.err) {
			
			
			
			o.L = nullptr;
			o.index = 0;
			o.returncount = 0;
			o.popcount = 0;
			o.err = load_status::syntax;
		}
		load_result& operator=(load_result&& o) noexcept {
			L = o.L;
			index = o.index;
			returncount = o.returncount;
			popcount = o.popcount;
			err = o.err;
			
			
			
			o.L = nullptr;
			o.index = 0;
			o.returncount = 0;
			o.popcount = 0;
			o.err = load_status::syntax;
			return *this;
		}

		load_status status() const noexcept {
			return err;
		}

		bool valid() const noexcept {
			return status() == load_status::ok;
		}

		template <typename T>
		T get() const {
			return tagged_get(types<meta::unqualified_t<T>>());
		}

		template <typename... Ret, typename... Args>
		decltype(auto) call(Args&&... args) {
#if !defined(__clang__) && defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 191200000
			
			return get<protected_function>().call<Ret...>(std::forward<Args>(args)...);
#else
			return get<protected_function>().template call<Ret...>(std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		decltype(auto) operator()(Args&&... args) {
			return call<>(std::forward<Args>(args)...);
		}

		lua_State* lua_state() const noexcept {
			return L;
		};
		int stack_index() const noexcept {
			return index;
		};

		~load_result() {
			stack::remove(L, index, popcount);
		}
	};
} 

#endif 
