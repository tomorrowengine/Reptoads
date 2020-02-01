// Unified Vignette Pixel shader
#include "VignetteShaderBuffers.h"

SamplerState samp0 : register(s0);

float4 main(VS_OUTPUT Input) : SEM_TARGET_OUTPUT
{
	float2 position = Input.UV - float2(0.5, 0.5);
	//fade out faster on x based on aspect ratio to get a spherical shape
	//position.x *= aspectRatio;
	const float dist = length(position);
	float vignette = smoothstep(vignetteRadius, vignetteRadius * .5 - vignetteSoftness, dist);

	const float3 vignetteColor = float3(0.0, 0.0, 0.0);
	return float4(vignetteColor, 1.0 - vignette);
}