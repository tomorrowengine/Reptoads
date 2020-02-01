






















#ifndef SOL_STACK_ITERATOR_HPP
#define SOL_STACK_ITERATOR_HPP

#include "stack.hpp"
#include <limits>
#include <iterator>

namespace sol {
	template <typename proxy_t, bool is_const>
	struct stack_iterator {
		typedef std::conditional_t<is_const, const proxy_t, proxy_t> reference;
		typedef std::conditional_t<is_const, const proxy_t*, proxy_t*> pointer;
		typedef proxy_t value_type;
		typedef std::ptrdiff_t difference_type;
		typedef std::random_access_iterator_tag iterator_category;
		lua_State* L;
		int index;
		int stacktop;
		proxy_t sp;

		stack_iterator()
			: L(nullptr), index((std::numeric_limits<int>::max)()), stacktop((std::numeric_limits<int>::max)()), sp() {
		}
		stack_iterator(const stack_iterator<proxy_t, true>& r)
			: L(r.L), index(r.index), stacktop(r.stacktop), sp(r.sp) {
		}
		stack_iterator(lua_State* luastate, int idx, int topidx)
			: L(luastate), index(idx), stacktop(topidx), sp(luastate, idx) {
		}

		reference operator*() {
			return proxy_t(L, index);
		}

		reference operator*() const {
			return proxy_t(L, index);
		}

		pointer operator->() {
			sp = proxy_t(L, index);
			return &sp;
		}

		pointer operator->() const {
			const_cast<proxy_t&>(sp) = proxy_t(L, index);
			return &sp;
		}

		stack_iterator& operator++() {
			++index;
			return *this;
		}

		stack_iterator operator++(int) {
			auto r = *this;
			this->operator++();
			return r;
		}

		stack_iterator& operator--() {
			--index;
			return *this;
		}

		stack_iterator operator--(int) {
			auto r = *this;
			this->operator--();
			return r;
		}

		stack_iterator& operator+=(difference_type idx) {
			index += static_cast<int>(idx);
			return *this;
		}

		stack_iterator& operator-=(difference_type idx) {
			index -= static_cast<int>(idx);
			return *this;
		}

		difference_type operator-(const stack_iterator& r) const {
			return index - r.index;
		}

		stack_iterator operator+(difference_type idx) const {
			stack_iterator r = *this;
			r += idx;
			return r;
		}

		reference operator[](difference_type idx) const {
			return proxy_t(L, index + static_cast<int>(idx));
		}

		bool operator==(const stack_iterator& r) const {
			if (stacktop == (std::numeric_limits<int>::max)()) {
				return r.index == r.stacktop;
			}
			else if (r.stacktop == (std::numeric_limits<int>::max)()) {
				return index == stacktop;
			}
			return index == r.index;
		}

		bool operator!=(const stack_iterator& r) const {
			return !(this->operator==(r));
		}

		bool operator<(const stack_iterator& r) const {
			return index < r.index;
		}

		bool operator>(const stack_iterator& r) const {
			return index > r.index;
		}

		bool operator<=(const stack_iterator& r) const {
			return index <= r.index;
		}

		bool operator>=(const stack_iterator& r) const {
			return index >= r.index;
		}
	};

	template <typename proxy_t, bool is_const>
	inline stack_iterator<proxy_t, is_const> operator+(typename stack_iterator<proxy_t, is_const>::difference_type n, const stack_iterator<proxy_t, is_const>& r) {
		return r + n;
	}
} 

#endif 