#ifdef __cplusplus
#pragma once
#endif

#include "ShaderDefines.h"

SHADER_cbuffer BlurData
{
	
	
	
	
	V2 dimensions;
	bool blurHorizontal;
	UINT mipLevel;
};
