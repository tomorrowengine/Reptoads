#pragma once
#include "memory/Allocator.h"

#include <map>
#include <unordered_map>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <stack>
#include <array>

#include <flat_value_map.h>
#include <flat_value_map_handle.h>



namespace ptl {
    template<
        typename Key,
        typename T,
        typename Hash = std::hash<Key>,
        typename KeyEqual = std::equal_to<Key>
    > 
    using unordered_map = std::unordered_map<Key,T,Hash, KeyEqual, MemoryAllocator<std::pair<const Key, T>>>;

	template<
		typename Key,
		typename T,
		typename Hash = std::hash<Key>,
		typename KeyEqual = std::equal_to<Key>
	>
	using unordered_multimap = std::unordered_multimap<Key, T, Hash, KeyEqual, MemoryAllocator<std::pair<const Key, T>>>;

	template<
		typename T,
		typename SparseHandle = cof::fvm_handle<T>
	>
	using flat_value_map = cof::flat_value_map<T, SparseHandle, MemoryAllocator<T>>;

    template<
        typename Key,
        typename T,
        typename Compare = std::less<Key>
    > 
    using map = std::map<Key,T, Compare, ptl::MemoryAllocator<std::pair<const Key, T> >>;

    template<
        class T
    >
    using deque = std::deque<T, ptl::MemoryAllocator<T>>;

    template<
        class T,
        class Container = ptl::deque<T>
    > 
    using  queue = std::queue<T, Container>;

	template<
		class T,
		class Container = ptl::deque<T>
	>
	using stack = std::stack<T, Container>;

    template<
        class T
    > 
    using vector = std::vector<T, ptl::MemoryAllocator<T>>;

	template<
		typename T,
		std::size_t Size
	>
	using array = std::array<T, Size>;

    template<
        class T
    > 
    using list = std::list<T, ptl::MemoryAllocator<T>>;

	template<
		class T,
		class Container = ptl::vector<T>,
		class Compare = std::less<typename Container::value_type>
	> using priority_queue = std::priority_queue<T,Container,Compare>;
}
