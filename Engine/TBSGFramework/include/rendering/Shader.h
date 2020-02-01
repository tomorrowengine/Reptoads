#pragma once
#include "memory/String.h"

namespace gfx
{
	enum ShaderType
	{
		
		PS = 1 << 0,
		
		VS = 1 << 1,
		
		CS = 1 << 2,
		
		SHADER_END_BIT = 1 << 3
	};

	class IShader
	{
	public:
		IShader(ShaderType type, ptl::string a_name) : type(type), name(a_name) {}
		virtual ~IShader() = default;

		const ShaderType type;
		const ptl::string name;
	};
}
