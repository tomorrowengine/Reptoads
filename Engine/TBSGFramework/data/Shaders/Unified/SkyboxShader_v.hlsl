#include "ShaderDefines.h"
#include "SkyboxBuffers.h"

struct CubemapShaderInput
{
    float3 Position : POSITION;
	float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct VertexShaderOutput
{
    float4 Position : SEM_POSITION;
    float3 TexCoord : TEXCOORD0;
};

VertexShaderOutput main(CubemapShaderInput IN)
{
    VertexShaderOutput OUT;

    OUT.Position = mul(MVPMatrix, float4(IN.Position, 1.0f));
 
    float4 VertexPosition = mul(ModelMatrix, float4(IN.Position, 1.0f));
    OUT.TexCoord = float4(VertexPosition - float4(CameraPosition, 1.0f)).xyz;
 
    return OUT;
}
