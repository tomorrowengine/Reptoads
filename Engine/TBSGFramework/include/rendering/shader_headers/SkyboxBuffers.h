#ifndef H_SKYBOX_BUFFERS
#define H_SKYBOX_BUFFERS
#include "ShaderDefines.h"

SHADER_cbuffer SkyboxBuffer SHADER_CONSTANT_REGISTER(0)
{
	M4 ModelMatrix;
	M4 MVPMatrix;
	V3 CameraPosition;
};


#endif