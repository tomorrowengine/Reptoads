#include "ShaderDefines.h"
#include "DefaultShaderBuffers.h"

SamplerState samp0 : register(s0);
Texture2D<float4> diffuseTexture : register(t0);
Texture2D<float3> emissiveTexture : register(t1);

struct PSOut
{
	float4 diffuse : SEM_TARGET_OUTPUT_(0);
	float4 emissive : SEM_TARGET_OUTPUT_(1);
};

static const float3 fogColor = float3( 0.4, 0.6, 0.9);
static const float fogDensity = 0.025;

PSOut main(VS_OUTPUT Input)
{
	uint2 pixel;
	pixel.x = (uint)Input.Position.x;
	pixel.y = (uint)Input.Position.y;

	float4 diffuseValue = diffuseTexture.Sample(samp0, Input.UV) * materialDiffuseColor;
	diffuseValue.a = (diffuseValue.a - alphaCutoff) / max(fwidth(diffuseValue.a), 0.0001) + 0.5;
	float3 emissiveValue = emissiveTexture.Sample(samp0, Input.UV) * materialEmissiveColor;

	float dist = -1000 - Input.PositionWS.y;

	float fogFactor = saturate((80.0 - dist) / (80.0 - 20.0));
	float3 finalColor = lerp(fogColor, diffuseValue.xyz, fogFactor);
	
	PSOut outValue;
	outValue.diffuse = float4(finalColor, diffuseValue.a);
	outValue.emissive = float4(emissiveValue.xyz,1.0);

	return outValue;
}
