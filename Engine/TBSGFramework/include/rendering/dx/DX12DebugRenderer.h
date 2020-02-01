#pragma once
#include <rendering/IDebugRenderer.h>

namespace gfx
{
	class DX12Renderer;

	class DX12DebugRenderer : public IDebugRenderer
	{
		DX12Renderer* m_renderer;

	public:
		DX12DebugRenderer(DX12Renderer* renderer);
		
		void Initialize() override;
		
		void AddLine() override;
		
		void Render() override;
		
		void Clear() override;
	};
}
