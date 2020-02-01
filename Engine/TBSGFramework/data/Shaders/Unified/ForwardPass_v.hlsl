#include "DefaultShaderBuffers.h"

struct VS_INPUT
{
	float3 Position		: POSITION0;
	//float3 Color		: COLOR0;
	float3 Normal		: NORMAL0;
	float2 UV			: TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.PositionWS = mul(modelMatrix, float4(Input.Position.xyz, 1.0)).xyz;
	Output.PositionVS = mul(viewMatrix, float4(Output.PositionWS.xyz, 1.0)).xyz;
	Output.Position = mul(vpMatrix, float4(Output.PositionWS.xyz, 1.0));
	Output.Normal = mul(normalMatrix, float4(Input.Normal.xyz, 1.0)).xyz;
	Output.CamPosWS = camPosWs.xyz;
	Output.UV = Input.UV;

	return Output;
}