






















#ifndef SOL_TABLE_HPP
#define SOL_TABLE_HPP

#include "table_core.hpp"

namespace sol {
	typedef table_core<false> table;

	namespace stack {
		template <>
		struct getter<metatable_t> {
			static table get(lua_State* L, int index = -1) {
				if (lua_getmetatable(L, index) == 0) {
					return table(L, ref_index(LUA_REFNIL));
				}
				return table(L, -1);
			}
		};
	} 
} 

#endif 
