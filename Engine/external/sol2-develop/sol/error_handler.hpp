






















#ifndef SOL_ERROR_HANDLER_HPP
#define SOL_ERROR_HANDLER_HPP

#include "types.hpp"
#include "demangle.hpp"

namespace sol {

	inline std::string associated_type_name(lua_State* L, int index, type t) {
		switch (t) {
		case type::poly:
			return "anything";
		case type::userdata:
		{
			if (lua_getmetatable(L, index) == 0) {
				break;
			}
			lua_pushlstring(L, "__name", 6);
			lua_rawget(L, -2);
			size_t sz;
			const char* name = lua_tolstring(L, -1, &sz);
			std::string tn(name, static_cast<std::string::size_type>(sz));
			lua_pop(L, 2);
			return name;
		}
		default:
			break;
		}
		return lua_typename(L, static_cast<int>(t));
	}

	inline int type_panic_string(lua_State* L, int index, type expected, type actual, const std::string& message = "") noexcept(false) {
		const char* err = message.empty() ? "stack index %d, expected %s, received %s" : "stack index %d, expected %s, received %s: %s";
		std::string actualname = associated_type_name(L, index, actual);
		return luaL_error(L, err, index,
			expected == type::poly ? "anything" : lua_typename(L, static_cast<int>(expected)),
			actualname.c_str(),
			message.c_str());
	}

	inline int type_panic_c_str(lua_State* L, int index, type expected, type actual, const char* message = nullptr) noexcept(false) {
		const char* err = message == nullptr || (std::char_traits<char>::length(message) == 0) ? "stack index %d, expected %s, received %s" : "stack index %d, expected %s, received %s: %s";
		std::string actualname = associated_type_name(L, index, actual);
		return luaL_error(L, err, index,
			expected == type::poly ? "anything" : lua_typename(L, static_cast<int>(expected)),
			actualname.c_str(),
			message);
	}

	struct type_panic_t {
		int operator()(lua_State* L, int index, type expected, type actual) const noexcept(false) {
			return type_panic_c_str(L, index, expected, actual, nullptr);
		}
		int operator()(lua_State* L, int index, type expected, type actual, const char* message) const noexcept(false) {
			return type_panic_c_str(L, index, expected, actual, message);
		}
		int operator()(lua_State* L, int index, type expected, type actual, const std::string& message) const noexcept(false) {
			return type_panic_string(L, index, expected, actual, message);
		}
	};

	const type_panic_t type_panic = {};

	struct constructor_handler {
		int operator()(lua_State* L, int index, type expected, type actual, const std::string& message) const noexcept(false) {
			std::string str = "(type check failed in constructor)";
			return type_panic_string(L, index, expected, actual, message.empty() ? str : message + " " + str);
		}
	};

	template <typename F = void>
	struct argument_handler {
		int operator()(lua_State* L, int index, type expected, type actual, const std::string& message) const noexcept(false) {
			std::string str = "(bad argument to variable or function call)";
			return type_panic_string(L, index, expected, actual, message.empty() ? str : message + " " + str );
		}
	};

	template <typename R, typename... Args>
	struct argument_handler<types<R, Args...>> {
		int operator()(lua_State* L, int index, type expected, type actual, const std::string& message) const noexcept(false) {
			std::string addendum = "(bad argument into '";
			addendum += detail::demangle<R>();
			addendum += "(";
			int marker = 0;
			auto action = [&addendum, &marker](const std::string& n) {
				if (marker > 0) {
					addendum += ", ";
				}
				addendum += n;
				++marker;
			};
			(void)detail::swallow{int(), (action(detail::demangle<Args>()), int())...};
			addendum += ")')";
			return type_panic_string(L, index, expected, actual, message.empty() ? addendum : message + " " + addendum);
		}
	};

	
	inline int no_panic(lua_State*, int, type, type, const char* = nullptr) noexcept {
		return 0;
	}

	inline void type_error(lua_State* L, int expected, int actual) noexcept(false) {
		luaL_error(L, "expected %s, received %s", lua_typename(L, expected), lua_typename(L, actual));
	}

	inline void type_error(lua_State* L, type expected, type actual) noexcept(false) {
		type_error(L, static_cast<int>(expected), static_cast<int>(actual));
	}

	inline void type_assert(lua_State* L, int index, type expected, type actual) noexcept(false) {
		if (expected != type::poly && expected != actual) {
			type_panic_c_str(L, index, expected, actual, nullptr);
		}
	}

	inline void type_assert(lua_State* L, int index, type expected) {
		type actual = type_of(L, index);
		type_assert(L, index, expected, actual);
	}

} 

#endif 
