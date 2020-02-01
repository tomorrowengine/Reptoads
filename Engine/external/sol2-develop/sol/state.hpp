






















#ifndef SOL_STATE_HPP
#define SOL_STATE_HPP

#include "state_view.hpp"
#include "thread.hpp"

namespace sol {

	class state : private std::unique_ptr<lua_State, detail::state_deleter>, public state_view {
	private:
		typedef std::unique_ptr<lua_State, detail::state_deleter> unique_base;

	public:
		state(lua_CFunction panic = default_at_panic)
		: unique_base(luaL_newstate()), state_view(unique_base::get()) {
			set_default_state(unique_base::get(), panic);
		}

		state(lua_CFunction panic, lua_Alloc alfunc, void* alpointer = nullptr)
		: unique_base(lua_newstate(alfunc, alpointer)), state_view(unique_base::get()) {
			set_default_state(unique_base::get(), panic);
		}

		state(const state&) = delete;
		state(state&&) = default;
		state& operator=(const state&) = delete;
		state& operator=(state&& that) {
			state_view::operator=(std::move(that));
			unique_base::operator=(std::move(that));
			return *this;
		}

		using state_view::get;

		~state() {
		}
	};
} 

#endif 
