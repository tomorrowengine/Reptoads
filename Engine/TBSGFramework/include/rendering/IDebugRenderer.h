#pragma once
#include <memory>

namespace gfx
{
	class IDebugRenderer
	{
	public:
		virtual ~IDebugRenderer() = default;
		
		virtual void Initialize() = 0;
		
		virtual void AddLine() = 0;
		
		virtual void Render() = 0;
		
		virtual void Clear() = 0;
	};
}
