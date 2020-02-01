#pragma once

#include "ShaderDefines.h"

SHADER_cbuffer ParticleVSData SHADER_CONSTANT_REGISTER(0)
{
	M4 ModelMatrix;
	M4 ViewMatrix;
	M4 ProjectionMatrix;
	M4 MVPMatrix;
};

#ifndef __cplusplus
struct VertexShaderOutput
{
	V4 Position : SEM_POSITION;
	V2 TexCoord : TEXCOORD0;
};
#endif