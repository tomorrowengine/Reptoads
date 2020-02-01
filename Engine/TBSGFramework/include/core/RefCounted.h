#pragma once
#include <cstdint>
#include <utility>
#include <core/Assertion.h>


template<typename T>
class ref_counted
{
	T value_;
	uint32_t ref_count_;

public:
	ref_counted() : value_({}), ref_count_(1)
	{}

	explicit ref_counted(const T& value) : value_(value), ref_count_(1)
	{}

	explicit ref_counted(T&& value) : value_(std::move(value)), ref_count_(1)
	{}

	ref_counted(const ref_counted&) = delete;
	ref_counted(ref_counted&& other) noexcept(std::is_nothrow_move_constructible<T>::value) 
		: value_(std::move(other.value_)), ref_count_(other.ref_count_)
	{}
	ref_counted& operator=(const ref_counted&) = delete;
	ref_counted& operator=(ref_counted&& other) noexcept(std::is_nothrow_move_constructible<T>::value)
	{
		value_ = std::move(other.value_);
		ref_count_ = other.ref_count_;

		return *this;
	}
	~ref_counted() = default;

	T& value() { return value_; }
	const T& value() const { return value_; }

	uint32_t ref_count() const { return ref_count_; }
	uint32_t decrease_ref_count() { 
		ASSERT(ref_count_ > 0);
		return --ref_count_; 
	}
	uint32_t increase_ref_count() { return ++ref_count_; }
};
