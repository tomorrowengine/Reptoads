#include "BloomShaderBuffers.h"

struct VS_INPUT
{
	float3 Position		: POSITION0;
	float2 UV			: TEXCOORD0;
};

VS_OUTPUT main( VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.Position = float4(Input.Position.xyz, 1.0);
	Output.UV = Input.UV;

	return Output;
};