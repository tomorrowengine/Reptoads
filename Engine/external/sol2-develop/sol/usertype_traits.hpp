






















#ifndef SOL_USERTYPE_TRAITS_HPP
#define SOL_USERTYPE_TRAITS_HPP

#include "demangle.hpp"

namespace sol {

	template <typename T>
	struct usertype_traits {
		static const std::string& name() {
			static const std::string& n = detail::short_demangle<T>();
			return n;
		}
		static const std::string& qualified_name() {
			static const std::string& q_n = detail::demangle<T>();
			return q_n;
		}
		static const std::string& metatable() {
			static const std::string m = std::string("sol.").append(detail::demangle<T>());
			return m;
		}
		static const std::string& user_metatable() {
			static const std::string u_m = std::string("sol.").append(detail::demangle<T>()).append(".user");
			return u_m;
		}
		static const std::string& user_gc_metatable() {
			static const std::string u_g_m = std::string("sol.").append(detail::demangle<T>()).append(".user\xE2\x99\xBB");
			return u_g_m;
		}
		static const std::string& gc_table() {
			static const std::string g_t = std::string("sol.").append(detail::demangle<T>()).append(".\xE2\x99\xBB");
			return g_t;
		}
	};

} 

#endif 
