#pragma once
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cassert>
#include <functional>

#include "utils/container_utils.h"
#include "flat_value_map_handle.h"


namespace cof
{
	


	
	template<typename T, typename SparseHandle = fvm_handle<T>,
		typename Allocator = std::allocator<T>,
		typename SparseToDenseAllocator = typename Allocator::template rebind< std::pair<const SparseHandle, uint32_t> >::other,
		typename DenseToSparseAllocator = typename Allocator::template rebind< std::pair<uint32_t, const SparseHandle> >::other
	>
		class flat_value_map
	{
	public:
		using handle_t = SparseHandle;

	private:
		using SparseToDenseMap = std::unordered_map<handle_t, std::size_t, std::hash<handle_t>, std::equal_to<>, SparseToDenseAllocator>;
		using SparseToDenseIterator = typename SparseToDenseMap::iterator;
		using DenseToSparseMap = std::unordered_map<std::size_t, handle_t, std::hash<std::size_t>, std::equal_to<>, DenseToSparseAllocator>;
		using DenseToSparseIterator = typename DenseToSparseMap::iterator;
		using DenseVector = std::vector<T, Allocator>;

		
		SparseToDenseMap sparse_to_dense{};
		
		DenseToSparseMap dense_to_sparse{};
		
		DenseVector dense_vector;

		SparseToDenseIterator back_element_sparse_to_dense_iterator;
		DenseToSparseIterator back_element_dense_to_sparse_iterator;
		bool back_element_cached_iterator_valid = false;

		static uint32_t internalIdCounter;

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = typename DenseVector::size_type;
		using difference_type = typename DenseVector::difference_type;
		using reverse_iterator = typename DenseVector::reverse_iterator;
		using const_reverse_iterator = typename DenseVector::const_reverse_iterator;

		using iterator = typename DenseVector::iterator;
		using const_iterator = typename DenseVector::const_iterator;
		using reference = typename DenseVector::reference;
		using const_reference = typename DenseVector::const_reference;
		using pointer = typename DenseVector::pointer;
		using const_pointer = typename DenseVector::const_pointer;

		using sparse_to_dense_iterator = typename SparseToDenseMap::iterator;
		using const_sparse_to_dense_iterator = typename SparseToDenseMap::const_iterator;

	public:
		flat_value_map() = default;

		

		
		auto operator[](handle_t handle)->reference;
		
		auto operator[](handle_t handle) const->const_reference;
		
		auto front()->reference;
		
		auto front() const->const_reference;
		
		auto back()->reference;
		
		auto back() const->const_reference;
		
		auto data()->pointer;
		
		auto data() const->const_pointer;

		
		bool contains(handle_t handle) const;
		
		auto find(handle_t handle)->iterator;
		
		auto find(handle_t handle) const->const_iterator;


		

		
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


		

		
		std::size_t size() const;
		
		bool empty() const;

		

		
		template<typename = std::enable_if_t<std::is_copy_constructible<T>::value>>
		auto push_back(const T& t)->handle_t
		{
			std::size_t element_index = dense_vector.size();
			uint32_t element_id = ++internalIdCounter;
			dense_vector.emplace_back(t);
			auto sparse_to_dense_it = unordered_map_emplace_and_return_iterator(sparse_to_dense, handle_t{ element_id }, element_index);
			auto dense_to_sparse_it = unordered_map_emplace_and_return_iterator(dense_to_sparse, element_index, handle_t{ element_id });
			back_element_sparse_to_dense_iterator = sparse_to_dense_it;
			back_element_dense_to_sparse_iterator = dense_to_sparse_it;
			back_element_cached_iterator_valid = true;

			return handle_t{ element_id };
		}
		
		auto push_back(T&& t)->handle_t;
		
		template<typename... Args>
		auto emplace_back(Args&&... args)->handle_t;

		
		void erase(handle_t handleToDelete);
		
		
		void erase(const_iterator position);
		
		
		void erase(const_iterator first, const_iterator last);

		
		void clear();
	};

#if defined(_WIN32) && __cplusplus >= 201703L
	namespace pmr {
		
		template<typename T, typename Allocator = std::experimental::pmr::polymorphic_allocator<T>>
		using flat_value_map = cof::flat_value_map<T, Allocator>;
	}
#endif
}






namespace cof
{
	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	uint32_t flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::internalIdCounter = 0;


	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::operator[](handle_t handle) -> reference
	{
		assert(sparse_to_dense.find(handle) != sparse_to_dense.end());
		auto element_index = sparse_to_dense.at(handle);
		assert(vector_in_range(dense_vector, element_index));
		return dense_vector[element_index];
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::operator[](
		handle_t handle) const -> const_reference
	{
		assert(sparse_to_dense.find(handle) != sparse_to_dense.end());
		auto element_index = sparse_to_dense.at(handle);
		assert(vector_in_range(dense_vector, element_index));
		return dense_vector[element_index];
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::front() -> reference
	{
		return dense_vector.front();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		front() const -> const_reference
	{
		return dense_vector.front();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::back() -> reference
	{
		return dense_vector.back();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		back() const -> const_reference
	{
		return dense_vector.back();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::data() -> pointer
	{
		return dense_vector.data();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		data() const -> const_pointer
	{
		return dense_vector.data();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	bool flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::contains(
		handle_t handle) const
	{
		return sparse_to_dense.find(handle) != sparse_to_dense.end();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::find(
		handle_t handle) -> iterator
	{
		auto sparse_to_dense_it = sparse_to_dense.find(handle);
		if (sparse_to_dense_it != sparse_to_dense.end()) {
			std::size_t element_index = sparse_to_dense_it->second;
			assert(vector_in_range(dense_vector, element_index));

			auto dense_it = dense_vector.begin();
			std::advance(dense_it, element_index);
			return dense_it;
		}

		return dense_vector.end();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::find(
		handle_t handle) const -> const_iterator
	{
		auto sparse_to_dense_it = sparse_to_dense.find(handle);
		if (sparse_to_dense_it != sparse_to_dense.end()) {
			std::size_t element_index = sparse_to_dense_it->second;
			assert(vector_in_range(dense_vector, element_index));

			auto dense_it = dense_vector.begin();
			std::advance(dense_it, element_index);
			return dense_it;
		}

		return dense_vector.end();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::begin() -> iterator
	{
		return dense_vector.begin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::begin() const -> const_iterator
	{
		return dense_vector.begin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		cbegin() const -> const_iterator
	{
		return dense_vector.cbegin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::end() -> iterator
	{
		return dense_vector.end();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::end() const -> const_iterator
	{
		return dense_vector.end();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		cend() const -> const_iterator
	{
		return dense_vector.cend();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::rbegin() -> iterator
	{
		return dense_vector.rbegin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		rbegin() const -> const_iterator
	{
		return dense_vector.rbegin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		crbegin() const -> const_iterator
	{
		return dense_vector.crbegin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::rend() -> iterator
	{
		return dense_vector.rend();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		rend() const -> const_iterator
	{
		return dense_vector.rend();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		crend() const -> const_iterator
	{
		return dense_vector.crbegin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		handles_begin() -> sparse_to_dense_iterator
	{
		return sparse_to_dense.begin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		handles_begin() const -> const_sparse_to_dense_iterator
	{
		return sparse_to_dense.begin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		handles_cbegin() const -> const_sparse_to_dense_iterator
	{
		return sparse_to_dense.cbegin();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		handles_end() -> sparse_to_dense_iterator
	{
		return sparse_to_dense.end();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		handles_end() const -> const_sparse_to_dense_iterator
	{
		return sparse_to_dense.end();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::
		handles_cend() const -> const_sparse_to_dense_iterator
	{
		return sparse_to_dense.cend();
	}


	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	std::size_t flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::size() const
	{
		return dense_vector.size();
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	bool flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::empty() const
	{
		return dense_vector.empty();
	}


	
	
	
	
	
	
	
	
	
	
	
	

	
	

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::push_back(T&& t) -> handle_t
	{
		std::size_t element_index = dense_vector.size();
		uint32_t element_id = ++internalIdCounter;
		dense_vector.push_back(std::move(t));
		auto sparse_to_dense_it = unordered_map_emplace_and_return_iterator(sparse_to_dense, handle_t{ element_id }, element_index);
		auto dense_to_sparse_it = unordered_map_emplace_and_return_iterator(dense_to_sparse, element_index, handle_t{ element_id });
		back_element_sparse_to_dense_iterator = sparse_to_dense_it;
		back_element_dense_to_sparse_iterator = dense_to_sparse_it;
		back_element_cached_iterator_valid = true;

		return handle_t{ element_id };
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	template <typename ... Args>
	auto flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::emplace_back(
		Args&&... args) -> handle_t
	{
		std::size_t element_index = dense_vector.size();
		uint32_t element_id = ++internalIdCounter;
		dense_vector.emplace_back(std::forward<Args>(args)...);
		auto sparse_to_dense_it = unordered_map_emplace_and_return_iterator(sparse_to_dense, handle_t{ element_id }, element_index);
		auto dense_to_sparse_it = unordered_map_emplace_and_return_iterator(dense_vector, element_index, handle_t{ element_id });
		back_element_sparse_to_dense_iterator = sparse_to_dense_it;
		back_element_dense_to_sparse_iterator = dense_to_sparse_it;
		back_element_cached_iterator_valid = true;

		return handle_t{ element_id };
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	void flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::erase(handle_t handleToDelete)
	{
		auto removing_sparse_to_dense_it = sparse_to_dense.find(handleToDelete);
		assert(removing_sparse_to_dense_it != sparse_to_dense.end());
		std::size_t removed_element_index = removing_sparse_to_dense_it->second;

		DenseToSparseIterator back_dts_it;
		if (removed_element_index != dense_vector.size() - 1) {
			
			SparseToDenseIterator back_std_it;

			if (back_element_cached_iterator_valid) {
				back_dts_it = back_element_dense_to_sparse_iterator;
				back_std_it = back_element_sparse_to_dense_iterator;
			} else {
				back_dts_it = dense_to_sparse.find(dense_vector.size() - 1);
				back_std_it = sparse_to_dense.find(back_dts_it->second);
			}

			assert(vector_in_range(dense_vector, removed_element_index));
			auto& removed_element = dense_vector[removed_element_index];
			auto& last_element = dense_vector.back();
			std::swap(removed_element, last_element);

			
			back_std_it->second = removed_element_index;
			dense_to_sparse.at(removed_element_index) = back_std_it->first;
		} else {
			if (back_element_cached_iterator_valid) {
				back_dts_it = back_element_dense_to_sparse_iterator;
			} else {
				back_dts_it = dense_to_sparse.find(dense_vector.size() - 1);
			}
		}
		sparse_to_dense.erase(removing_sparse_to_dense_it);
		dense_to_sparse.erase(back_dts_it);
		dense_vector.pop_back();

		back_element_cached_iterator_valid = false;
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	void flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::erase(
		const_iterator position)
	{
		std::size_t element_index = position - dense_vector.begin();
		
		assert(dense_to_sparse.find(element_index) != dense_to_sparse.end());
		handle_t handle = dense_to_sparse.at(element_index);

		erase(handle);
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	void flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::erase(
		const_iterator first, const_iterator last)
	{
		

		auto count = last - first;
		std::vector<handle_t> handles(count);

		auto offset = first - begin();
		for (int i = 0; i < count; ++i) {
			auto dense_to_sparse_it = dense_to_sparse.find(offset + i);
			assert(dense_to_sparse_it != dense_to_sparse.end());
			handles[i] = dense_to_sparse_it->second;
		}

		for (handle_t handle : handles) {
			erase(handle);
		}
	}

	template<typename T, typename SparseHandle, typename Allocator, typename SparseToDenseAllocator, typename DenseToSparseAllocator>
	void flat_value_map<T, SparseHandle, Allocator, SparseToDenseAllocator, DenseToSparseAllocator>::clear()
	{
		dense_vector.clear();
		sparse_to_dense.clear();
		dense_to_sparse.clear();
	}


	
}
