#include "UIOverlayShaderBuffers.h"
Texture2D UITexture : register(t0);
SamplerState TextureSampler : register(s0);

float4 main(VS_OUTPUT IN ) : SEM_TARGET_OUTPUT
{
	return UITexture.Sample(TextureSampler, IN.UV) * textColor;
}
