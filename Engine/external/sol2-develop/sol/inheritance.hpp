






















#ifndef SOL_INHERITANCE_HPP
#define SOL_INHERITANCE_HPP

#include "types.hpp"
#include "usertype_traits.hpp"

namespace sol {
	template <typename... Args>
	struct base_list {};
	template <typename... Args>
	using bases = base_list<Args...>;

	typedef bases<> base_classes_tag;
	const auto base_classes = base_classes_tag();

	namespace detail {

		template <typename T>
		struct has_derived {
			static bool value;
		};

		template <typename T>
		bool has_derived<T>::value = false;

		inline decltype(auto) base_class_check_key() {
			static const auto& key = "class_check";
			return key;
		}

		inline decltype(auto) base_class_cast_key() {
			static const auto& key = "class_cast";
			return key;
		}

		inline decltype(auto) base_class_index_propogation_key() {
			static const auto& key = u8"\xF0\x9F\x8C\xB2.index";
			return key;
		}

		inline decltype(auto) base_class_new_index_propogation_key() {
			static const auto& key = u8"\xF0\x9F\x8C\xB2.new_index";
			return key;
		}

		template <typename T, typename... Bases>
		struct inheritance {
			static bool type_check_bases(types<>, const std::string&) {
				return false;
			}

			template <typename Base, typename... Args>
			static bool type_check_bases(types<Base, Args...>, const std::string& ti) {
				return ti == usertype_traits<Base>::qualified_name() || type_check_bases(types<Args...>(), ti);
			}

			static bool type_check(const std::string& ti) {
				return ti == usertype_traits<T>::qualified_name() || type_check_bases(types<Bases...>(), ti);
			}

			static void* type_cast_bases(types<>, T*, const std::string&) {
				return nullptr;
			}

			template <typename Base, typename... Args>
			static void* type_cast_bases(types<Base, Args...>, T* data, const std::string& ti) {
				
				return ti != usertype_traits<Base>::qualified_name() ? type_cast_bases(types<Args...>(), data, ti) : static_cast<void*>(static_cast<Base*>(data));
			}

			static void* type_cast(void* voiddata, const std::string& ti) {
				T* data = static_cast<T*>(voiddata);
				return static_cast<void*>(ti != usertype_traits<T>::qualified_name() ? type_cast_bases(types<Bases...>(), data, ti) : data);
			}

			template <typename U>
			static bool type_unique_cast_bases(void*, void*, const string_view&) {
				return false;
			}

			template <typename U, typename Base, typename... Args>
			static bool type_unique_cast_bases(void* source_data, void* target_data, const string_view& ti) {
				typedef unique_usertype_traits<U> uu_traits;
				typedef typename uu_traits::template rebind_base<Base> base_ptr;
				string_view base_ti = usertype_traits<Base>::qualified_name();
				if (base_ti == ti) {
					if (target_data != nullptr) {
						U* source = static_cast<U*>(source_data);
						base_ptr* target = static_cast<base_ptr*>(target_data);
						
						*target = *source;
					}
					return true;
				}
				return type_unique_cast_bases<U, Args...>(source_data, target_data, ti);
			}

			template <typename U>
			static bool type_unique_cast(void* source_data, void* target_data, const string_view& ti, const string_view& rebind_ti) {
				typedef unique_usertype_traits<U> uu_traits;
				typedef typename uu_traits::template rebind_base<T> rebind_t;
				string_view this_rebind_ti = usertype_traits<rebind_t>::qualified_name();
				if (rebind_ti != this_rebind_ti) {
					
					return false;
				}
				return type_unique_cast_bases<U, Bases...>(source_data, target_data, ti);
			}
		};

		using inheritance_check_function = decltype(&inheritance<void>::type_check);
		using inheritance_cast_function = decltype(&inheritance<void>::type_cast);
		using inheritance_unique_cast_function = decltype(&inheritance<void>::type_unique_cast<void>);
	} 
} 

#endif 
