#pragma once
#include <vector>
#include <unordered_map>
#include "utils/defines.h"




template<typename T, typename Allocator>
bool vector_in_range(const std::vector<T, Allocator>& vector, size_t index)
{
	return index >= 0 && index < vector.size();
}

template<typename T, typename Allocator, typename... Args>
NO_DISCARD auto vector_emplace_back_and_return_iterator(std::vector<T, Allocator>& vector, Args&&... args) -> typename std::vector<T, Allocator>::iterator
{
#if __cplusplus >= 201703L
	
	return vector.emplace_back(std::forward<Args>(args)...);
#else
	
	vector.emplace_back(std::forward<Args>(args)...);
	return --vector.end();
#endif
}

template<typename T, typename Allocator>
NO_DISCARD auto vector_push_back_and_return_iterator(std::vector<T, Allocator>& vector, const T& value) -> typename std::vector<T, Allocator>::iterator
{
#if __cplusplus >= 201703L
	
	return vector.push_back(value);
#else
	
	vector.push_back(value);
	return --vector.end();
#endif
}

template<typename T, typename Allocator>
NO_DISCARD auto vector_push_back_and_return_iterator(std::vector<T, Allocator>& vector, T&& value) -> typename std::vector<T, Allocator>::iterator
{
#if __cplusplus >= 201703L
	
	return vector.push_back(value);
#else
	
	vector.push_back(value);
	return --vector.end();
#endif
}


template<typename T, typename E, typename Hasher, typename KeyEq, typename Allocator, typename... Args>
NO_DISCARD auto unordered_map_emplace_and_return_iterator(std::unordered_map<T, E, Hasher, KeyEq, Allocator>& map, Args&&... args) 
	-> typename std::unordered_map<T, E, Hasher, KeyEq, Allocator>::iterator
{
	auto iterator_and_success = map.emplace(std::forward<Args>(args)...);
	assert(iterator_and_success.second);
	return iterator_and_success.first;
}


template<typename T, typename E, typename Hasher, typename KeyEq, typename Allocator, typename... Args>
NO_DISCARD auto unordered_map_emplace_and_return_iterator_no_check(std::unordered_map<T, E, Hasher, KeyEq, Allocator>& map, Args&&... args)
	-> typename std::unordered_map<T, E, Hasher, KeyEq, Allocator>::iterator
{
	auto iterator_and_success = map.emplace(std::forward<Args>(args)...);
	return iterator_and_success.first;
}
