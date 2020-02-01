#include "BlurShaderBuffers.h"
Texture2D<float4> blurTexInput			: register(t0);
RW_Texture2D<float4> blurTexOutput		: register(u0);
SamplerState samp						: register(s0);
#define OFFSET_1 1.3846153846
#define OFFSET_2 3.2307692308
#define WEIGHT_0 0.2270270270
#define WEIGHT_1 0.3162162162
#define WEIGHT_2 0.0702702703

void StoreBlurMipPixel (uint mipLevel, uint2 pixel, float4 blurColor)
{
#ifdef __PSSL__
	blurTexOutput.MipMaps(mipLevel, pixel) = blurColor;
#else
	blurTexOutput[pixel] = blurColor;
#endif
}

//blur data (from Osmium)
// the number of threads per wavefront
[NUM_THREADS(8,8,1)]
void main(uint2 globalThreadId : SEM_DISPATCH_THREAD_ID)
{
	const float2 pixelZeroToOne = ((float2)globalThreadId + float2(0.5, 0.5)) / float2(dimensions.x, dimensions.y);
	float weight = 1.0f / 5.0f;

	if(mipLevel > 0)
	{
		float4 blurColor = blurTexInput.SampleLOD(samp, pixelZeroToOne, (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne - float2(OFFSET_1 / dimensions.x, 0.0f), (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne + float2(OFFSET_1 / dimensions.x, 0.0f), (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne - float2(OFFSET_2 / dimensions.x, 0.0f), (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne + float2(OFFSET_2 / dimensions.x, 0.0f), (float)mipLevel) * weight;
		StoreBlurMipPixel(mipLevel, globalThreadId, blurColor);
	}

	// blur horizontally
	if(blurHorizontal) // blurHorizontal
	{
		float4 blurColor = blurTexInput.SampleLOD(samp, pixelZeroToOne, (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne - float2(OFFSET_1 / dimensions.x, 0.0f), (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne + float2(OFFSET_1 / dimensions.x, 0.0f), (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne - float2(OFFSET_2 / dimensions.x, 0.0f), (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne + float2(OFFSET_2 / dimensions.x, 0.0f), (float)mipLevel) * weight;
		StoreBlurMipPixel(mipLevel, globalThreadId, blurColor);
	}
//
	////blur vertically
	else
	{
		float4 blurColor = blurTexInput.SampleLOD(samp, pixelZeroToOne, (float)mipLevel) * weight;//* 0.2270270270;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne - float2(0.0f, OFFSET_1 / dimensions.y), (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne + float2(0.0f, OFFSET_1 / dimensions.y), (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne - float2(0.0f, OFFSET_2 / dimensions.y), (float)mipLevel) * weight;
		blurColor += blurTexInput.SampleLOD(samp, pixelZeroToOne + float2(0.0f, OFFSET_2 / dimensions.y), (float)mipLevel) * weight;
		StoreBlurMipPixel(mipLevel, globalThreadId, blurColor);
	}
};