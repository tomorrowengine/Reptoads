#define ENABLE_LIGHT_SPARSE_TO_DENSE_VECTOR_FIND
#include "light_flat_value_map.h"
#include "catch2/catch.hpp"
#include <string>
#include <utility>

using namespace cof;

struct TestValue
{
	int health;
	std::string name;


	friend bool operator==(const TestValue& lhs, const TestValue& rhs)
	{
		return lhs.health == rhs.health
			&& lhs.name == rhs.name;
	}

	friend bool operator!=(const TestValue& lhs, const TestValue& rhs) { return !(lhs == rhs); }
};

TEST_CASE("light_flat_value_map::find test")
{
	light_flat_value_map<TestValue> flat_value_map{};

	TestValue goblin{ 50, "Goblin" };
	TestValue little_goblin{ 10, "Little Goblin" };
	TestValue boss{ 150, "Boss" };

	auto goblin_handle = flat_value_map.push_back(goblin);
	auto lgoblin_handle = flat_value_map.push_back(little_goblin);
	auto boss_handle = flat_value_map.push_back(boss);

	REQUIRE(flat_value_map.size() == 3);

	CHECK_FALSE(*flat_value_map.find(goblin_handle) == little_goblin);
	CHECK(*flat_value_map.find(lgoblin_handle) == little_goblin);
	CHECK(*flat_value_map.find(goblin_handle) == goblin);
	CHECK(*flat_value_map.find(boss_handle) == boss);
}
TEST_CASE("const light_flat_value_map::find test")
{
	light_flat_value_map<TestValue> flat_value_map{};

	TestValue goblin{ 50, "Goblin" };
	TestValue little_goblin{ 10, "Little Goblin" };
	TestValue boss{ 150, "Boss" };

	auto goblin_handle = flat_value_map.push_back(goblin);
	auto lgoblin_handle = flat_value_map.push_back(little_goblin);
	auto boss_handle = flat_value_map.push_back(boss);

	const light_flat_value_map<TestValue>& const_flat_value_map = flat_value_map;

	REQUIRE(const_flat_value_map.size() == 3);

	CHECK_FALSE(*const_flat_value_map.find(goblin_handle) == little_goblin);
	CHECK(*const_flat_value_map.find(lgoblin_handle) == little_goblin);
	CHECK(*const_flat_value_map.find(goblin_handle) == goblin);
	CHECK(*const_flat_value_map.find(boss_handle) == boss);
}
