






















#ifndef SOL_STACK_PROXY_BASE_HPP
#define SOL_STACK_PROXY_BASE_HPP

#include "stack.hpp"
#include "proxy_base.hpp"

namespace sol {
	struct stack_proxy_base : public proxy_base<stack_proxy_base> {
	private:
		lua_State* L;
		int index;

	public:
		stack_proxy_base()
			: L(nullptr), index(0) {
		}
		stack_proxy_base(lua_State* L, int index)
			: L(L), index(index) {
		}

		template <typename T>
		decltype(auto) get() const {
			return stack::get<T>(L, stack_index());
		}

		template <typename T>
		bool is() const {
			return stack::check<T>(L, stack_index());
		}

		template <typename T>
		decltype(auto) as() const {
			return get<T>();
		}

		type get_type() const noexcept {
			return type_of(lua_state(), stack_index());
		}

		int push() const {
			return push(L);
		}

		int push(lua_State* Ls) const {
			lua_pushvalue(Ls, index);
			return 1;
		}

		lua_State* lua_state() const {
			return L;
		}
		int stack_index() const {
			return index;
		}
	};

	namespace stack {
		template <>
		struct getter<stack_proxy_base> {
			static stack_proxy_base get(lua_State* L, int index = -1) {
				return stack_proxy_base(L, index);
			}
		};

		template <>
		struct pusher<stack_proxy_base> {
			static int push(lua_State*, const stack_proxy_base& ref) {
				return ref.push();
			}
		};
	} 

} 

#endif 
