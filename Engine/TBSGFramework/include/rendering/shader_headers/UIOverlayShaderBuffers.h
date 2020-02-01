#pragma once
#include "ShaderDefines.h"

SHADER_cbuffer UIObjectConstantsVs SHADER_CONSTANT_REGISTER(0)
{
	M4 projectionMatrix;
};

SHADER_cbuffer UIObjectConstantsPs SHADER_CONSTANT_REGISTER(1)
{
	V4 textColor;
};

#ifndef __cplusplus
struct VS_OUTPUT
{
	float4 Position : SEM_POSITION;
	float2 UV : TEXCOORD0;
};
#endif