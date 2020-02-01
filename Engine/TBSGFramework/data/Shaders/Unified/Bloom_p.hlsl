// DeferredFinal_p.pssl
#include "BloomShaderBuffers.h"

SamplerState samp0 : register(s0);
#ifdef __PSSL__
Texture2D backBufferTexture : register(t0);
#endif
Texture2D bloomTexture : register(t1);

float4 main(VS_OUTPUT Input) : SEM_TARGET_OUTPUT
{
#ifdef __PSSL__
	float2 pixel;
	pixel.x = Input.Position.x;
	pixel.y = Input.Position.y;

	float3 backBufferColor = backBufferTexture[(uint2)pixel].xyz;
	float3 blurColor = float3(0,0,0);

	float mipWeight = 1.0 / ((finalBlurMip + 1) - startMipLevel);

	for(float i = startMipLevel; i < finalBlurMip; ++i)
	{
		blurColor = bloomTexture.SampleLOD(samp0, Input.UV, i).xyz;
		backBufferColor += blurColor * mipWeight * bloomIntensity;
	}

	return float4(backBufferColor, 1.0f);
#else
	float3 blurColor = float3(0, 0, 0);

	float mipWeight = 1.0 / ((finalBlurMip + 1) - startMipLevel);

	for (float i = startMipLevel; i < finalBlurMip; ++i) {
		blurColor += bloomTexture.SampleLOD(samp0, Input.UV, i).xyz * mipWeight * bloomIntensity;
	}

	return float4(blurColor, 1.0f);
#endif
}

