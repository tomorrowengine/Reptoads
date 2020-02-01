






















#ifndef SOL_STACK_QUALIFIED_GET_HPP
#define SOL_STACK_QUALIFIED_GET_HPP

#include "stack_get_unqualified.hpp"

namespace sol {
namespace stack {

#if 0 
	template <typename X>
	struct qualified_getter<X, std::enable_if_t<
		!std::is_reference<X>::value && is_unique_usertype<meta::unqualified_t<X>>::value
	>> {
		typedef typename unique_usertype_traits<meta::unqualified_t<X>>::type P;
		typedef typename unique_usertype_traits<meta::unqualified_t<X>>::actual_type Real;

		static Real& get(lua_State* L, int index, record& tracking) {
			tracking.use(1);
			void* memory = lua_touserdata(L, index);
			void* del = detail::align_usertype_unique_destructor(memory);
			memory = detail::align_usertype_unique<Real>(memory);
			Real* mem = static_cast<Real*>(memory);
			return *mem;
		}
	};
#endif 

	template <typename T>
	struct qualified_getter<T, std::enable_if_t<
		!std::is_reference<T>::value 
		&& is_container<meta::unqualified_t<T>>::value 
		&& std::is_default_constructible<meta::unqualified_t<T>>::value
		&& !is_lua_primitive<T>::value 
		&& !is_transparent_argument<T>::value 
	>> {
		static T get(lua_State* L, int index, record& tracking) {
			if (type_of(L, index) == type::userdata) {
				return stack_detail::unchecked_unqualified_get<T>(L, index, tracking);
			}
			else {
				return stack_detail::unchecked_unqualified_get<sol::nested<T>>(L, index, tracking);
			}
		}
	};
}
} 

#endif 
