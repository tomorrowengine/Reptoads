






















#ifndef SOL_DEBUG_HPP
#define SOL_DEBUG_HPP

#include "stack.hpp"
#include <iostream>

namespace sol {
namespace detail {
namespace debug {
	inline std::string dump_types(lua_State* L) {
		std::string visual;
		std::size_t size = lua_gettop(L) + 1;
		for (std::size_t i = 1; i < size; ++i) {
			if (i != 1) {
				visual += " | ";
			}
			visual += type_name(L, stack::get<type>(L, static_cast<int>(i)));
		}
		return visual;
	}

	inline void print_stack(lua_State* L) {
		std::cout << dump_types(L) << std::endl;
	}

	inline void print_section(const std::string& message, lua_State* L) {
		std::cout << "-- " << message << " -- [ " << dump_types(L) << " ]" << std::endl;
	}
}
}
} 

#endif 
