






















#ifndef SOL_STACK_CHECK_QUALIFIED_GET_HPP
#define SOL_STACK_CHECK_QUALIFIED_GET_HPP

#include "stack_core.hpp"
#include "stack_check_get_unqualified.hpp"

namespace sol {
namespace stack {
	template <typename T, typename C>
	struct qualified_check_getter : check_getter<meta::unqualified_t<T>, C> {};
}
} 

#endif 
