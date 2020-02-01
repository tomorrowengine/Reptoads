#ifdef PLATFORM_WINDOWS
#include <rendering/dx/DX12Shader.h>
#include <utility>


namespace gfx
{
	DX12Shader::DX12Shader(Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob, ShaderType type, std::string name)
		: IShader(type, name), shaderBlob(std::move(shaderBlob))
	{
	}
}
#endif
