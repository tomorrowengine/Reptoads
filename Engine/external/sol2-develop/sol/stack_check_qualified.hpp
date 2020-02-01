






















#ifndef SOL_STACK_CHECK_QUALIFIED_HPP
#define SOL_STACK_CHECK_QUALIFIED_HPP

#include "stack_check_unqualified.hpp"

namespace sol {
namespace stack {

#if 0
	template <typename X>
	struct qualified_checker<X, type::userdata, std::enable_if_t<is_unique_usertype<X>::value && !std::is_reference<X>::value>> {
		typedef unique_usertype_traits<meta::unqualified_t<X>> u_traits;
		typedef typename u_traits::type T;
		
		template <typename Handler>
		static bool check(std::false_type, lua_State* L, int index, Handler&& handler, record& tracking) {
			return stack::unqualified_check<X>(L, index, std::forward<Handler>(handler), tracking);
		}

		template <typename Handler>
		static bool check(std::true_type, lua_State* L, int index, Handler&& handler, record& tracking) {
			
			
			const type indextype = type_of(L, index);
			tracking.use(1);
			if (indextype != type::userdata) {
				handler(L, index, type::userdata, indextype, "value is not a userdata");
				return false;
			}
			if (lua_getmetatable(L, index) == 0) {
				return true;
			}
			int metatableindex = lua_gettop(L);
			void* basecastdata = lua_touserdata(L, index);
			void* memory = detail::align_usertype_unique_destructor(basecastdata);
			detail::unique_destructor& pdx = *static_cast<detail::unique_destructor*>(memory);
			if (&detail::usertype_unique_alloc_destroy<T, X> == pdx) {
				return true;
			}
			if (detail::has_derived<T>::value) {
				memory = detail::align_usertype_unique_cast<true>(memory);
				detail::inheritance_unique_cast_function ic = reinterpret_cast<detail::inheritance_unique_cast_function>(memory);
				string_view ti = usertype_traits<T>::qualified_name();
				string_view rebind_ti = usertype_traits<base_id>::qualified_name();
				if (ic(nullptr, basecastdata, ti, rebind_ti)) {
					lua_pop(L, 1);
				}
			}
			handler(L, index, type::userdata, indextype, "value is a userdata but is not the correct unique usertype");
			return false;
		}
		
		template <typename Handler>
		static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
			return check(meta::neg<std::is_void<typename u_traits::base_id>>(), L, index, std::forward<Handler>(handler), tracking);
		}
	};

#endif 

	template <typename X>
	struct qualified_checker<X, type::userdata, std::enable_if_t<is_container<meta::unqualified_t<X>>::value && !std::is_reference<X>::value>> {
		template <typename Handler>
		static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
			if (type_of(L, index) == type::userdata) {
				return stack::unqualified_check<X>(L, index, std::forward<Handler>(handler), tracking);
			}
			else {
				return stack::unqualified_check<nested<X>>(L, index, std::forward<Handler>(handler), tracking);
			}
		}
	};
}
} 

#endif 
