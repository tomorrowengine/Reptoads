






















#ifndef SOL_FORWARD_DETAIL_HPP
#define SOL_FORWARD_DETAIL_HPP

#include "feature_test.hpp"
#include "forward.hpp"
#include "traits.hpp"

namespace sol {
	namespace detail {
		const bool default_safe_function_calls =
#if defined(SOL_SAFE_FUNCTION_CALLS) && SOL_SAFE_FUNCTION_CALLS
			true;
#else
			false;
#endif
	} 


	namespace meta {
	namespace meta_detail {
	}
	} 

	namespace stack {
	namespace stack_detail {
		template <typename T>
		struct undefined_metatable;
	}
	} 

	namespace usertype_detail {
		template <typename T, typename Regs, typename Fx>
		void insert_default_registrations(Regs& l, int& index, Fx&& fx);

		template <typename T, typename Regs, meta::enable<meta::neg<std::is_pointer<T>>, std::is_destructible<T>> = meta::enabler>
		void make_destructor(Regs& l, int& index);
		template <typename T, typename Regs, meta::disable<meta::neg<std::is_pointer<T>>, std::is_destructible<T>> = meta::enabler>
		void make_destructor(Regs& l, int& index);
	} 
} 

#endif 
