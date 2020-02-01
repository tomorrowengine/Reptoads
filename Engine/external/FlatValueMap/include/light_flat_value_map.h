#pragma once
#include <memory>
#include <unordered_map>
#include <cassert>

#include "utils/container_utils.h"
#include "flat_value_map_handle.h"


namespace cof
{
	


	
	template<typename T, typename Allocator = std::allocator<T>, typename SparseToDenseAllocator = Allocator>
	class light_flat_value_map
	{
	public:
		using handle_t = lfvm_handle<T>;

	private:
		using SparseToDenseMap = std::unordered_map<handle_t, std::size_t, std::hash<handle_t>, std::equal_to<>, Allocator>;
		using SparseToDenseIterator = typename SparseToDenseMap::iterator;
		using DenseVector = std::vector<T, Allocator>;
		using DenseVectorIterator = typename DenseVector::iterator;

		SparseToDenseMap sparse_to_dense{};
		DenseVector dense_vector;

		static uint32_t internalIdCounter;

	public:
		using iterator = typename DenseVector::iterator;
		using const_iterator = typename DenseVector::const_iterator;
		using reference = typename DenseVector::reference;
		using const_reference = typename DenseVector::const_reference;
		using pointer = typename DenseVector::pointer;
		using const_pointer = typename DenseVector::const_pointer;

		using sparse_to_dense_iterator = typename SparseToDenseMap::iterator;
		using const_sparse_to_dense_iterator = typename SparseToDenseMap::const_iterator;

	public:
		light_flat_value_map() = default;


		

		
		auto operator[](handle_t handle)->reference;
		
		auto operator[](handle_t handle) const->const_reference;

		
		auto front()->reference;
		
		auto front() const->const_reference;
		
		auto back()->reference;
		
		auto back() const->const_reference;
		
		auto data()->pointer;
		
		auto data() const->const_pointer;

		
		bool contains(handle_t handle) const;
		
#ifdef ENABLE_LIGHT_SPARSE_TO_DENSE_VECTOR_FIND
		auto find(handle_t handle)->iterator;
		auto find(handle_t handle) const->const_iterator;
#endif 


		

		
		auto begin()->iterator;
		
		auto begin() const->const_iterator;
		
		auto cbegin() const->const_iterator;
		
		auto end()->iterator;
		
		auto end() const->const_iterator;
		
		auto cend() const->const_iterator;
		
		auto rbegin()->iterator;
		
		auto rbegin() const->const_iterator;
		
		auto crbegin() const->const_iterator;
		
		auto rend()->iterator;
		
		auto rend() const->const_iterator;
		
		auto crend() const->const_iterator;

		

		
		auto handles_begin()->sparse_to_dense_iterator;
		
		auto handles_begin() const->const_sparse_to_dense_iterator;
		
		auto handles_cbegin() const->const_sparse_to_dense_iterator;
		
		auto handles_end()->sparse_to_dense_iterator;
		
		auto handles_end() const->const_sparse_to_dense_iterator;
		
		auto handles_cend() const->const_sparse_to_dense_iterator;

		
		

		
		size_t size() const;
		
		bool empty() const;


		

		
		auto push_back(const T& t)->handle_t;
		
		auto push_back(T&& t)->handle_t;
		
		template<typename... Args>
		auto emplace_back(Args&&... args)->handle_t;

		
		void erase(handle_t handleToRemove);

		
		void clear();
		
	private:
		
		auto dense_to_sparse(std::size_t denseIndex)->SparseToDenseIterator;
	};

#if __cplusplus > 201703L
	namespace pmr {
		
		template<typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
		using light_flat_value_map = cof::light_flat_value_map<T, Allocator>;
}
#endif
}







namespace cof
{
	template<typename T, typename Allocator, typename SparseToDenseAllocator>
	uint32_t light_flat_value_map<T, Allocator, SparseToDenseAllocator>::internalIdCounter = 0;


	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::operator[](handle_t handle) -> reference {
		auto std_it = sparse_to_dense.find(handle);
		assert(std_it != sparse_to_dense.end());
		std::size_t element_index = std_it->second;
		assert(vector_in_range(dense_vector, element_index));
		return dense_vector[element_index];
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::operator[](
		handle_t handle) const -> const_reference {

		auto std_it = sparse_to_dense.find(handle);
		assert(std_it != sparse_to_dense.end());
		std::size_t element_index = std_it->second;
		assert(vector_in_range(dense_vector, element_index));
		return dense_vector[element_index];
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::front() -> reference
	{
		return dense_vector.front();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::front() const -> const_reference
	{
		return dense_vector.front();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::back() -> reference
	{
		return dense_vector.back();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::back() const -> const_reference
	{
		return dense_vector.back();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::data() -> pointer
	{
		return dense_vector.data();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::data() const -> const_pointer
	{
		return dense_vector.data();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	bool light_flat_value_map<T, Allocator, SparseToDenseAllocator>::contains(handle_t handle) const
	{
		return sparse_to_dense.find(handle) != sparse_to_dense.end();
	}

#ifdef ENABLE_LIGHT_SPARSE_TO_DENSE_VECTOR_FIND
	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::find(handle_t handle) -> iterator
	{
		auto sparse_to_dense_it = sparse_to_dense.find(handle);
		if (sparse_to_dense_it != sparse_to_dense.end()) {
			std::size_t element_index = sparse_to_dense_it->second;
			assert(vector_in_range(dense_vector, element_index));

			auto dense_it = dense_vector.begin();
			std::advance(dense_it, element_index);
			return dense_it;
		} else {
			return dense_vector.end();
		}
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::find(
		handle_t handle) const -> const_iterator
	{
		auto sparse_to_dense_it = sparse_to_dense.find(handle);
		if (sparse_to_dense_it != sparse_to_dense.end()) {
			std::size_t element_index = sparse_to_dense_it->second;
			assert(vector_in_range(dense_vector, element_index));

			auto dense_it = dense_vector.begin();
			std::advance(dense_it, element_index);
			return dense_it;
		} else {
			return dense_vector.end();
		}
	}
#endif 


	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::begin() -> iterator
	{
		return dense_vector.begin();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::begin() const -> const_iterator
	{
		return dense_vector.begin();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::cbegin() const -> const_iterator
	{
		return dense_vector.cbegin();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::end() -> iterator
	{
		return dense_vector.end();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::end() const -> const_iterator
	{
		return dense_vector.end();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::cend() const -> const_iterator
	{
		return dense_vector.cend();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::rbegin() -> iterator
	{
		return dense_vector.rbegin();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::rbegin() const -> const_iterator
	{
		return dense_vector.rbegin();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::crbegin() const -> const_iterator
	{
		return dense_vector.crbegin();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::rend() -> iterator
	{
		return dense_vector.rend();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::rend() const -> const_iterator
	{
		return dense_vector.rend();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::crend() const -> const_iterator
	{
		return dense_vector.crend();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::
		handles_begin() -> sparse_to_dense_iterator
	{
		return sparse_to_dense.begin();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::
		handles_begin() const -> const_sparse_to_dense_iterator
	{
		return sparse_to_dense.begin();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::
		handles_cbegin() const -> const_sparse_to_dense_iterator
	{
		return sparse_to_dense.cbegin();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::
		handles_end() -> sparse_to_dense_iterator
	{
		return sparse_to_dense.end();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::
		handles_end() const -> const_sparse_to_dense_iterator
	{
		return sparse_to_dense.end();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::
		handles_cend() const -> const_sparse_to_dense_iterator
	{
		return sparse_to_dense.cend();
	}


	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	size_t light_flat_value_map<T, Allocator, SparseToDenseAllocator>::size() const
	{
		return dense_vector.size();
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	bool light_flat_value_map<T, Allocator, SparseToDenseAllocator>::empty() const
	{
		return dense_vector.empty();
	}


	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::push_back(const T& t) -> handle_t
	{
		std::size_t element_index = dense_vector.size();
		uint32_t element_id = ++internalIdCounter;
		dense_vector.push_back(t);
		sparse_to_dense.emplace(handle_t{ element_id }, element_index);

		return handle_t{ element_id };
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::push_back(T&& t) -> handle_t
	{
		std::size_t element_index = dense_vector.size();
		uint32_t element_id = ++internalIdCounter;
		dense_vector.push_back(std::move(t));
		sparse_to_dense.emplace(handle_t{element_id}, element_index);

		return handle_t{element_id};
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	template <typename ... Args>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::emplace_back(Args&&... args) -> handle_t
	{
		std::size_t element_index = dense_vector.size();
		uint32_t element_id = ++internalIdCounter;
		dense_vector.emplace_back(std::forward<Args>(args)...);
		sparse_to_dense.emplace(handle_t{ element_id }, element_index);

		return handle_t{ element_id };
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	void light_flat_value_map<T, Allocator, SparseToDenseAllocator>::erase(handle_t handleToRemove)
	{
		auto sparse_to_dense_it = sparse_to_dense.find(handleToRemove);
		assert(sparse_to_dense_it != sparse_to_dense.end());
		if (sparse_to_dense_it != sparse_to_dense.end()) {
			std::size_t element_index = sparse_to_dense_it->second;

			if (element_index != dense_vector.size() - 1) {
				
				std::size_t last_element = dense_vector.size() - 1;
				std::swap(dense_vector[element_index], dense_vector[last_element]);
				auto std_last_element_it = dense_to_sparse(last_element);
				std_last_element_it->second = element_index;
			}
			dense_vector.pop_back();
			sparse_to_dense.erase(sparse_to_dense_it);
		}
	}

	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	void light_flat_value_map<T, Allocator, SparseToDenseAllocator>::clear()
	{
		dense_vector.clear();
		sparse_to_dense.clear();
	}


	template <typename T, typename Allocator, typename SparseToDenseAllocator>
	auto light_flat_value_map<T, Allocator, SparseToDenseAllocator>::dense_to_sparse(
		std::size_t denseIndex) -> SparseToDenseIterator
	{
		for (auto it = sparse_to_dense.begin(); it != sparse_to_dense.end(); ++it) {
			if(denseIndex == it->second) {
				return it;
			}
		}
		assert(false);
		return sparse_to_dense.end();
	}


	
}
