#include <catch2/catch.hpp>
#include "flat_value_map.h"
#include "light_flat_value_map.h"


template<typename T>
class PassThroughAllocator
{
	using StdAllocator = std::allocator<T>;
	StdAllocator internalAllocator;

public:
	using value_type = typename StdAllocator::value_type;
	using pointer = typename StdAllocator::pointer;
	using const_pointer = typename StdAllocator::const_pointer;
	using reference = typename StdAllocator::reference;
	using const_reference = typename StdAllocator::const_reference;
	using size_type = typename StdAllocator::size_type;
	using difference_type = typename StdAllocator::difference_type;
	using propagate_on_container_move_assignment = typename StdAllocator::propagate_on_container_move_assignment;
	template< class U > 
	struct rebind { typedef PassThroughAllocator<U> other; };
	using is_always_equal = typename StdAllocator::is_always_equal;


	PassThroughAllocator() : internalAllocator() {}
	template<class _Other>
	PassThroughAllocator(const PassThroughAllocator<_Other>&) noexcept
	{	
	}

	PassThroughAllocator(const PassThroughAllocator& other) = default;
	PassThroughAllocator(PassThroughAllocator&& other) noexcept = default;
	PassThroughAllocator& operator=(const PassThroughAllocator& other) = default;
	PassThroughAllocator& operator=(PassThroughAllocator&& other) noexcept = default;

	T* allocate(std::size_t n)
	{
		return internalAllocator.allocate(n);
	}

	T* allocate(std::size_t n, const void * hint)
	{
		return internalAllocator.allocate(n, hint);
	}

	void deallocate(T* p, std::size_t n)
	{
		internalAllocator.deallocate(p, n);
	}

	friend bool operator==(const PassThroughAllocator& lhs, const PassThroughAllocator& rhs)
	{
		return lhs.internalAllocator == rhs.internalAllocator;
	}

	friend bool operator!=(const PassThroughAllocator& lhs, const PassThroughAllocator& rhs) { return !(lhs == rhs); }
};

struct TestType
{
	int i = 0;
	std::string name = "hello world!";

	TestType() = default;
	TestType(int i, const std::string& name) : i(i), name(name) {}
};

TEST_CASE("Custom allocator test")
{
	cof::flat_value_map<TestType, cof::fvm_handle<TestType>, PassThroughAllocator<TestType>> fvm{};

	TestType tValue{};
	auto copyHandle = fvm.push_back(tValue);
	auto moveHandle = fvm.push_back(std::move(tValue));
	auto emplacedHandle = fvm.emplace_back(100, std::string{ "Another name" });
	CHECK(fvm.contains(copyHandle));
	CHECK(fvm.contains(moveHandle));
	CHECK(fvm.contains(emplacedHandle));

	CHECK(fvm.size() == 3);
	CHECK(fvm.find(moveHandle) != fvm.end());
}
