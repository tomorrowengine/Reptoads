






















#ifndef SOL_PROTECTED_FUNCTION_RESULT_HPP
#define SOL_PROTECTED_FUNCTION_RESULT_HPP

#include "reference.hpp"
#include "tuple.hpp"
#include "stack.hpp"
#include "proxy_base.hpp"
#include "stack_iterator.hpp"
#include "stack_proxy.hpp"
#include "error.hpp"
#include "stack.hpp"
#include <cstdint>

namespace sol {
	struct protected_function_result : public proxy_base<protected_function_result> {
	private:
		lua_State* L;
		int index;
		int returncount;
		int popcount;
		call_status err;

		template <typename T>
		decltype(auto) tagged_get(types<optional<T>>, int index_offset) const {
			typedef decltype(stack::get<optional<T>>(L, index)) ret_t;
			int target = index + index_offset;
			if (!valid()) {
				return ret_t(nullopt);
			}
			return stack::get<optional<T>>(L, target);
		}

		template <typename T>
		decltype(auto) tagged_get(types<T>, int index_offset) const {
			int target = index + index_offset;
#if defined(SOL_SAFE_PROXIES) && SOL_SAFE_PROXIES
			if (!valid()) {
				type t = type_of(L, target);
				type_panic_c_str(L, target, t, type::none, "bad get from protected_function_result (is not an error)");
			}
#endif 
			return stack::get<T>(L, target);
		}

		optional<error> tagged_get(types<optional<error>>, int index_offset) const {
			int target = index + index_offset;
			if (valid()) {
				return nullopt;
			}
			return error(detail::direct_error, stack::get<std::string>(L, target));
		}

		error tagged_get(types<error>, int index_offset) const {
			int target = index + index_offset;
#if defined(SOL_SAFE_PROXIES) && SOL_SAFE_PROXIES
			if (valid()) {
				type t = type_of(L, target);
				type_panic_c_str(L, target, t, type::none, "bad get from protected_function_result (is an error)");
			}
#endif 
			return error(detail::direct_error, stack::get<std::string>(L, target));
		}

	public:
		typedef stack_proxy reference_type;
		typedef stack_proxy value_type;
		typedef stack_proxy* pointer;
		typedef std::ptrdiff_t difference_type;
		typedef std::size_t size_type;
		typedef stack_iterator<stack_proxy, false> iterator;
		typedef stack_iterator<stack_proxy, true> const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		protected_function_result() = default;
		protected_function_result(lua_State* Ls, int idx = -1, int retnum = 0, int popped = 0, call_status pferr = call_status::ok) noexcept
		: L(Ls), index(idx), returncount(retnum), popcount(popped), err(pferr) {
		}
		protected_function_result(const protected_function_result&) = default;
		protected_function_result& operator=(const protected_function_result&) = default;
		protected_function_result(protected_function_result&& o) noexcept
		: L(o.L), index(o.index), returncount(o.returncount), popcount(o.popcount), err(o.err) {
			
			
			
			o.abandon();
		}
		protected_function_result& operator=(protected_function_result&& o) noexcept {
			L = o.L;
			index = o.index;
			returncount = o.returncount;
			popcount = o.popcount;
			err = o.err;
			
			
			
			o.abandon();
			return *this;
		}

		protected_function_result(const unsafe_function_result& o) = delete;
		protected_function_result& operator=(const unsafe_function_result& o) = delete;
		protected_function_result(unsafe_function_result&& o) noexcept;
		protected_function_result& operator=(unsafe_function_result&& o) noexcept;

		call_status status() const noexcept {
			return err;
		}

		bool valid() const noexcept {
			return status() == call_status::ok || status() == call_status::yielded;
		}

		template <typename T>
		decltype(auto) get(int index_offset = 0) const {
			return tagged_get(types<meta::unqualified_t<T>>(), index_offset);
		}

		type get_type(difference_type index_offset = 0) const noexcept {
			return type_of(L, index + static_cast<int>(index_offset));
		}

		stack_proxy operator[](difference_type index_offset) const {
			return stack_proxy(L, index + static_cast<int>(index_offset));
		}

		iterator begin() {
			return iterator(L, index, stack_index() + return_count());
		}
		iterator end() {
			return iterator(L, stack_index() + return_count(), stack_index() + return_count());
		}
		const_iterator begin() const {
			return const_iterator(L, index, stack_index() + return_count());
		}
		const_iterator end() const {
			return const_iterator(L, stack_index() + return_count(), stack_index() + return_count());
		}
		const_iterator cbegin() const {
			return begin();
		}
		const_iterator cend() const {
			return end();
		}

		reverse_iterator rbegin() {
			return std::reverse_iterator<iterator>(begin());
		}
		reverse_iterator rend() {
			return std::reverse_iterator<iterator>(end());
		}
		const_reverse_iterator rbegin() const {
			return std::reverse_iterator<const_iterator>(begin());
		}
		const_reverse_iterator rend() const {
			return std::reverse_iterator<const_iterator>(end());
		}
		const_reverse_iterator crbegin() const {
			return std::reverse_iterator<const_iterator>(cbegin());
		}
		const_reverse_iterator crend() const {
			return std::reverse_iterator<const_iterator>(cend());
		}

		lua_State* lua_state() const noexcept {
			return L;
		};
		int stack_index() const noexcept {
			return index;
		};
		int return_count() const noexcept {
			return returncount;
		};
		int pop_count() const noexcept {
			return popcount;
		};
		void abandon() noexcept {
			
			index = 0;
			returncount = 0;
			popcount = 0;
			err = call_status::runtime;
		}
		~protected_function_result() {
			stack::remove(L, index, popcount);
		}
	};

	namespace stack {
		template <>
		struct pusher<protected_function_result> {
			static int push(lua_State* L, const protected_function_result& pfr) {
				int p = 0;
				for (int i = 0; i < pfr.pop_count(); ++i) {
					lua_pushvalue(L, i + pfr.stack_index());
					++p;
				}
				return p;
			}
		};
	} 
} 

#endif 
