






















#ifndef SOL_TABLE_ITERATOR_HPP
#define SOL_TABLE_ITERATOR_HPP

#include "object.hpp"
#include <iterator>

namespace sol {

	template <typename reference_type>
	class basic_table_iterator : public std::iterator<std::input_iterator_tag, std::pair<object, object>> {
	public:
		typedef object key_type;
		typedef object mapped_type;
		typedef std::pair<object, object> value_type;
		typedef std::input_iterator_tag iterator_category;
		typedef std::ptrdiff_t difference_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

	private:
		std::pair<object, object> kvp;
		reference_type ref;
		int tableidx = 0;
		int keyidx = 0;
		std::ptrdiff_t idx = 0;

	public:
		basic_table_iterator()
		: keyidx(-1), idx(-1) {
		}

		basic_table_iterator(reference_type x)
		: ref(std::move(x)) {
			ref.push();
			tableidx = lua_gettop(ref.lua_state());
			stack::push(ref.lua_state(), lua_nil);
			this->operator++();
			if (idx == -1) {
				return;
			}
			--idx;
		}

		basic_table_iterator& operator++() {
			if (idx == -1)
				return *this;

			if (lua_next(ref.lua_state(), tableidx) == 0) {
				idx = -1;
				keyidx = -1;
				return *this;
			}
			++idx;
			kvp.first = object(ref.lua_state(), -2);
			kvp.second = object(ref.lua_state(), -1);
			lua_pop(ref.lua_state(), 1);
			
			keyidx = lua_gettop(ref.lua_state());
			return *this;
		}

		basic_table_iterator operator++(int) {
			auto saved = *this;
			this->operator++();
			return saved;
		}

		reference operator*() {
			return kvp;
		}

		const_reference operator*() const {
			return kvp;
		}

		bool operator==(const basic_table_iterator& right) const {
			return idx == right.idx;
		}

		bool operator!=(const basic_table_iterator& right) const {
			return idx != right.idx;
		}

		~basic_table_iterator() {
			if (keyidx != -1) {
				stack::remove(ref.lua_state(), keyidx, 1);
			}
			if (ref.lua_state() != nullptr && ref.valid()) {
				stack::remove(ref.lua_state(), tableidx, 1);
			}
		}
	};

} 

#endif 
