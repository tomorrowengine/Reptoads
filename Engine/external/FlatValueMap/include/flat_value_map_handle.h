#pragma once
#include <functional>

namespace cof
{
	
	template<typename T>
	struct fvm_handle {
		uint32_t id;

		friend bool operator==(const fvm_handle& lhs, const fvm_handle& rhs) { return lhs.id == rhs.id; }
		friend bool operator!=(const fvm_handle& lhs, const fvm_handle& rhs) { return lhs.id != rhs.id; }
		friend bool operator<(const fvm_handle& lhs, const fvm_handle& rhs) { return lhs.id < rhs.id; }
		friend bool operator<=(const fvm_handle& lhs, const fvm_handle& rhs) { return lhs.id <= rhs.id; }
		friend bool operator>(const fvm_handle& lhs, const fvm_handle& rhs) { return lhs.id > rhs.id; }
		friend bool operator>=(const fvm_handle& lhs, const fvm_handle& rhs) { return lhs.id >= rhs.id; }
	};

	
	template<typename T>
	struct lfvm_handle {
		uint32_t id;

		friend bool operator==(const lfvm_handle& lhs, const lfvm_handle& rhs) { return lhs.id == rhs.id; }
		friend bool operator!=(const lfvm_handle& lhs, const lfvm_handle& rhs) { return lhs.id != rhs.id; }
		friend bool operator<(const lfvm_handle& lhs, const lfvm_handle& rhs) { return lhs.id < rhs.id; }
		friend bool operator<=(const lfvm_handle& lhs, const lfvm_handle& rhs) { return lhs.id <= rhs.id; }
		friend bool operator>(const lfvm_handle& lhs, const lfvm_handle& rhs) { return lhs.id > rhs.id; }
		friend bool operator>=(const lfvm_handle& lhs, const lfvm_handle& rhs) { return lhs.id >= rhs.id; }
	};
}

namespace std
{
	template<typename T>
	struct hash<cof::fvm_handle<T>>
	{
		std::size_t operator()(const cof::fvm_handle<T>& handle) const
		{
			using internal_id_t = decltype(handle.id);

			return std::hash<internal_id_t>{}(handle.id);
		}
	};

	template<typename T>
	struct hash<cof::lfvm_handle<T>>
	{
		std::size_t operator()(const cof::lfvm_handle<T>& handle) const
		{
			using internal_id_t = decltype(handle.id);

			return std::hash<internal_id_t>{}(handle.id);
		}
	};
}
