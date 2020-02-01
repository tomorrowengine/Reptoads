






















#ifndef SOL_STACK_PROBE_HPP
#define SOL_STACK_PROBE_HPP

#include "stack_core.hpp"
#include "stack_field.hpp"
#include "stack_check.hpp"

namespace sol {
namespace stack {
	template <typename T, typename P, bool b, bool raw, typename>
	struct probe_field_getter {
		template <typename Key>
		probe get(lua_State* L, Key&& key, int tableindex = -2) {
			if (!b && !maybe_indexable(L, tableindex)) {
				return probe(false, 0);
			}
			get_field<b, raw>(L, std::forward<Key>(key), tableindex);
			return probe(check<P>(L), 1);
		}
	};

	template <typename A, typename B, typename P, bool b, bool raw, typename C>
	struct probe_field_getter<std::pair<A, B>, P, b, raw, C> {
		template <typename Keys>
		probe get(lua_State* L, Keys&& keys, int tableindex = -2) {
			if (!b && !maybe_indexable(L, tableindex)) {
				return probe(false, 0);
			}
			get_field<b, raw>(L, std::get<0>(keys), tableindex);
			if (!maybe_indexable(L)) {
				return probe(false, 1);
			}
			get_field<false, raw>(L, std::get<1>(keys), tableindex);
			return probe(check<P>(L), 2);
		}
	};

	template <typename... Args, typename P, bool b, bool raw, typename C>
	struct probe_field_getter<std::tuple<Args...>, P, b, raw, C> {
		template <std::size_t I, typename Keys>
		probe apply(std::index_sequence<I>, int sofar, lua_State* L, Keys&& keys, int tableindex) {
			get_field < I<1 && b, raw>(L, std::get<I>(keys), tableindex);
			return probe(check<P>(L), sofar);
		}

		template <std::size_t I, std::size_t I1, std::size_t... In, typename Keys>
		probe apply(std::index_sequence<I, I1, In...>, int sofar, lua_State* L, Keys&& keys, int tableindex) {
			get_field < I<1 && b, raw>(L, std::get<I>(keys), tableindex);
			if (!maybe_indexable(L)) {
				return probe(false, sofar);
			}
			return apply(std::index_sequence<I1, In...>(), sofar + 1, L, std::forward<Keys>(keys), -1);
		}

		template <typename Keys>
		probe get(lua_State* L, Keys&& keys, int tableindex = -2) {
			if (!b && !maybe_indexable(L, tableindex)) {
				return probe(false, 0);
			}
			return apply(std::index_sequence_for<Args...>(), 1, L, std::forward<Keys>(keys), tableindex);
		}
	};
}
} 

#endif 
