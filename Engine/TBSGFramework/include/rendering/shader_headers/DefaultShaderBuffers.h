#ifndef H_DEFAULT_SHADER_BUFFERS
#define H_DEFAULT_SHADER_BUFFERS
#include "ShaderDefines.h"


SHADER_cbuffer PerFrameConstantsVs SHADER_CONSTANT_REGISTER(0)
{
	M4 vpMatrix;
	M4 viewMatrix;
	V4 camPosWs;
};

SHADER_cbuffer PerObjectConstantsVs SHADER_CONSTANT_REGISTER(1)
{
	M4 modelMatrix;
	M4 normalMatrix;
};


SHADER_cbuffer ShaderMaterial SHADER_CONSTANT_REGISTER(2)
{
	V4 materialDiffuseColor;
	V3 materialEmissiveColor;
	float alphaCutoff;
};



#ifndef __cplusplus

struct VS_OUTPUT
{
	float4 Position : SEM_POSITION;
	float3 PositionWS : POSITIONWS;
	float3 PositionVS : POSITIONVS;
	float3 Normal : NORMAL1;
	float3 CamPosWS : CAMPOSWS;
	float2 UV : TEXCOORD1;
};

#endif

#endif 
