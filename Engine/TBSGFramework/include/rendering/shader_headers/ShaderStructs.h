#pragma once
#include "glm/gtc/type_ptr.hpp"


namespace gfx
{
	struct V2
	{
		float x{};
		float y{};
		V2 &operator=(const glm::vec2& rhs)
		{
			memcpy(this, glm::value_ptr(rhs), sizeof(*this));
			return *this;
		}
	};

	struct V3
	{
		float x{};
		float y{};
		float z{};
		V3 &operator=(const glm::vec3& rhs)
		{
			memcpy(this, glm::value_ptr(rhs), sizeof(*this));
			return *this;
		}
	};

	struct V4
	{
		float x{};
		float y{};
		float z{};
		float w{};

		V4 &operator=(const glm::vec4 &rhs)
		{
			memcpy(this, glm::value_ptr(rhs), sizeof(*this));
			return *this;
		}
		float& operator[](size_t rhs)
		{
			return *(&x + rhs);
		}
	};

	
	struct M4
	{
		V4 x{};
		V4 y{};
		V4 z{};
		V4 w{};
		M4 &operator=(const glm::mat4 &rhs)
		{
			memcpy(this, glm::value_ptr(rhs), sizeof(*this));
			return *this;
		}
	};
}
