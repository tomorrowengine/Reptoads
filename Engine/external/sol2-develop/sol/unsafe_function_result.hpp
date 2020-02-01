






















#ifndef SOL_UNSAFE_FUNCTION_RESULT_HPP
#define SOL_UNSAFE_FUNCTION_RESULT_HPP

#include "reference.hpp"
#include "tuple.hpp"
#include "stack.hpp"
#include "proxy_base.hpp"
#include "stack_iterator.hpp"
#include "stack_proxy.hpp"
#include <cstdint>

namespace sol {
	struct unsafe_function_result : public proxy_base<unsafe_function_result> {
	private:
		lua_State* L;
		int index;
		int returncount;

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

		unsafe_function_result() = default;
		unsafe_function_result(lua_State* Ls, int idx = -1, int retnum = 0)
			: L(Ls), index(idx), returncount(retnum) {
		}
		unsafe_function_result(const unsafe_function_result&) = default;
		unsafe_function_result& operator=(const unsafe_function_result&) = default;
		unsafe_function_result(unsafe_function_result&& o)
			: L(o.L), index(o.index), returncount(o.returncount) {
			
			
			
			o.abandon();
		}
		unsafe_function_result& operator=(unsafe_function_result&& o) {
			L = o.L;
			index = o.index;
			returncount = o.returncount;
			
			
			
			o.abandon();
			return *this;
		}

		unsafe_function_result(const protected_function_result& o) = delete;
		unsafe_function_result& operator=(const protected_function_result& o) = delete;
		unsafe_function_result(protected_function_result&& o) noexcept;
		unsafe_function_result& operator=(protected_function_result&& o) noexcept;

		template <typename T>
		decltype(auto) get(difference_type index_offset = 0) const {
			return stack::get<T>(L, index + static_cast<int>(index_offset));
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

		call_status status() const noexcept {
			return call_status::ok;
		}

		bool valid() const noexcept {
			return status() == call_status::ok || status() == call_status::yielded;
		}

		lua_State* lua_state() const {
			return L;
		};
		int stack_index() const {
			return index;
		};
		int return_count() const {
			return returncount;
		};
		void abandon() noexcept {
			
			index = 0;
			returncount = 0;
		}
		~unsafe_function_result() {
			lua_pop(L, returncount);
		}
	};

	namespace stack {
		template <>
		struct pusher<unsafe_function_result> {
			static int push(lua_State* L, const unsafe_function_result& fr) {
				int p = 0;
				for (int i = 0; i < fr.return_count(); ++i) {
					lua_pushvalue(L, i + fr.stack_index());
					++p;
				}
				return p;
			}
		};
	} 
} 

#endif 
