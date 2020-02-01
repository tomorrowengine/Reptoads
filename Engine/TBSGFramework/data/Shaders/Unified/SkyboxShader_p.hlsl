#include "ShaderDefines.h"

struct VertexShaderOutput
{
    float4 Position : SEM_POSITION;
    float3 TexCoord : TEXCOORD0;
};

TextureCube CubeTexture : register(t0);
SamplerState LinearMirrorSampler : register(s0);

float4 main( VertexShaderOutput IN ) : SEM_TARGET_OUTPUT
{
	float4 texColor = CubeTexture.SampleLOD(LinearMirrorSampler, normalize(IN.TexCoord), 0.0f);
	texColor.a = 1.0f;
    return texColor;
}
