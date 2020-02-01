#pragma once
#include "memory/Containers.h"

namespace ptl {

	template<typename K, typename V>
	class sparse_set
	{
	public:

		sparse_set() = default;
		sparse_set(std::size_t size)
		{
			reserve(size);

		}
		sparse_set(const vector<K>& keys, const vector<V>& values)
			: keys(keys),
			values(values)
		{
		}

		sparse_set(vector<K> keys, vector<V> values)
			: keys(std::move(keys)),
			values(std::move(values))
		{
		}

		sparse_set(const sparse_set& other) = default;
		sparse_set(sparse_set&& other) = default;
		sparse_set& operator=(const sparse_set& other) = default;
		sparse_set& operator=(sparse_set&& other) = default;

		void reserve(std::size_t size);
		void insert(K&& key, V&& value);
		void insert(K&& key, V& value);
		void insert(K& key, V& value);
		void insert(K& key, V&& value);

		bool has(const K& key) const;

		V& at(const K& key);
		const V& at(const K& key) const;

		void clear();
		std::size_t size() const;
		bool empty() const;
		std::size_t capacity();
		auto begin() const;
		auto end() const;

		vector<K> keys{};
		vector<V> values{};
	};

	template <typename K, typename V>
	void sparse_set<K, V>::reserve(std::size_t size)
	{
		keys.reserve(size);
		values.reserve(size);
	}

	template <typename K, typename V>
	void sparse_set<K, V>::insert(K&& key, V&& value)
	{
		keys.emplace_back(std::move(key));
		values.emplace_back(std::move(value));
	}

	template <typename K, typename V>
	void sparse_set<K, V>::insert(K&& key, V& value)
	{
		keys.emplace_back(std::move(key));
		values.emplace_back(value);
	}

	template <typename K, typename V>
	void sparse_set<K, V>::insert(K& key, V& value)
	{
		keys.emplace_back(key);
		values.emplace_back(value);
	}

	template <typename K, typename V>
	void sparse_set<K, V>::insert(K& key, V&& value)
	{
		keys.emplace_back(key);
		values.emplace_back(std::move(value));
	}

	template <typename K, typename V>
	bool sparse_set<K, V>::has(const K& key) const
	{
		for (auto i = 0; i < keys.size(); i++)
		{
			if (keys[i] == key)
				return true;
		}
		return false;
	}

	template <typename K, typename V>
	V& sparse_set<K, V>::at(const K& key)
	{
		for (auto i = 0; i < keys.size(); i++)
		{
			if (keys[i] == key)
				return values[i];
		}
		assert(false &&"Key could not be found");
		throw std::out_of_range{ "Key could not be found" };
	}

	template <typename K, typename V>
	const V& sparse_set<K, V>::at(const K& key) const
	{
		for (auto i = 0; i < keys.size(); i++)
		{
			if (keys[i] == key)
				return values[i];
		}
		assert(false &&"Key could not be found");
		throw std::out_of_range{ "Key could not be found" };
	}


	template <typename K, typename V>
	void sparse_set<K, V>::clear()
	{
		keys.clear();
		values.clear();
	}



	template <typename K, typename V>
	std::size_t sparse_set<K, V>::size() const
	{
		return keys.size();
	}

	template <typename K, typename V>
	bool sparse_set<K, V>::empty() const
	{
		return keys.empty();
	}

	template <typename K, typename V>
	std::size_t sparse_set<K, V>::capacity()
	{
		return values.capacity();
	}

	template <typename K, typename V>
	auto sparse_set<K, V>::begin() const
	{
		return values.begin();
	}

	template <typename K, typename V>
	auto sparse_set<K, V>::end() const
	{
		return values.end();
	}
}
