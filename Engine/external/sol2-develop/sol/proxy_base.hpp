






















#ifndef SOL_PROXY_BASE_HPP
#define SOL_PROXY_BASE_HPP

#include "reference.hpp"
#include "tuple.hpp"
#include "stack.hpp"

namespace sol {
	struct proxy_base_tag {};

	template <typename Super>
	struct proxy_base : proxy_base_tag {
		operator std::string() const {
			const Super& super = *static_cast<const Super*>(static_cast<const void*>(this));
			return super.template get<std::string>();
		}

		template <typename T, meta::enable<meta::neg<meta::is_string_constructible<T>>, is_proxy_primitive<meta::unqualified_t<T>>> = meta::enabler>
		operator T() const {
			const Super& super = *static_cast<const Super*>(static_cast<const void*>(this));
			return super.template get<T>();
		}

		template <typename T, meta::enable<meta::neg<meta::is_string_constructible<T>>, meta::neg<is_proxy_primitive<meta::unqualified_t<T>>>> = meta::enabler>
		operator T&() const {
			const Super& super = *static_cast<const Super*>(static_cast<const void*>(this));
			return super.template get<T&>();
		}

		lua_State* lua_state() const {
			const Super& super = *static_cast<const Super*>(static_cast<const void*>(this));
			return super.lua_state();
		}
	};
} 

#endif 
