#pragma once
#ifdef PLATFORM_WINDOWS
#include <rendering/Shader.h>
#include <rendering/dx/d3dx12/d3dx12.h>

#include <wrl.h>
#include <string>

namespace gfx
{
	class DX12Shader : public IShader
	{
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;

	public:
		DX12Shader(Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob, ShaderType type, std::string name);

		ID3DBlob* getShaderBlob() const { return shaderBlob.Get(); }
	};
}
#endif
