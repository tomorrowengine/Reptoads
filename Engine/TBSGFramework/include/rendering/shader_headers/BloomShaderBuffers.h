#ifdef __cplusplus
#pragma once
#endif

#include "ShaderDefines.h"

SHADER_cbuffer BloomData
{
	UINT startMipLevel;
	UINT finalBlurMip;
	float bloomIntensity;
};

#ifndef __cplusplus
struct VS_OUTPUT
{
	float4 Position : SEM_POSITION;
	float2 UV : TEXCOORD1;
};
#endif
