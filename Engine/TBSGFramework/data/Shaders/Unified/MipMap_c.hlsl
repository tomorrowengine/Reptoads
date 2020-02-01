#include "MipMapShaderBuffers.h"
Texture2D<float4> srcMip		: register(t0);
RW_Texture2D<float4> outMip		: register(u0);
SamplerState samp			: register(s0);


// inherited from the Dx12 implementation
// 64 threads in wavefront
thread_group_memory float shared_R[64];
thread_group_memory float shared_G[64];
thread_group_memory float shared_B[64];
thread_group_memory float shared_A[64];

void SaveSharedColor(uint threadId, float4 color)
{
	shared_R[threadId] = color.x;
	shared_G[threadId] = color.y;
	shared_B[threadId] = color.z;
	shared_A[threadId] = color.w;
}

float4 LoadSharedColor(uint threadId)
{
	return float4(shared_R[threadId], shared_G[threadId], shared_B[threadId], shared_A[threadId]);
}

// inherited from the Dx12 implementation
// Source: https://en.wikipedia.org/wiki/SRGB#The_forward_transformation_(CIE_XYZ_to_sRGB)
float3 ConvertToSrgb(float3 a_color)
{
	return a_color < 0.0031308 ? 12.92 * a_color : 1.055 * pow(abs(a_color), 1.0 / 2.4) - 0.055;
}

float4 GetColor(float4 a_color)
{
	if(!isSrgb) return a_color;
	return float4(ConvertToSrgb(a_color.xyz), a_color.w);
}

// the number of threads per wavefront
[NUM_THREADS(8,8,1)]
void main(uint threadInGroup : S_GROUP_INDEX, uint2 globalThreadId : S_DISPATCH_THREAD_ID)
{
	float4 color = float4(0,0,0,0);

	if(texDesc == TEX_DESC_WIDTH_HEIGHT_EVEN)
	{
		//get the interpolated value between the 4 pixels
		float2 uv = texelSize * (globalThreadId + float2(0.5, 0.5));
		color = srcMip.SampleLOD(samp, uv, srcMipLevel);
	}

	else if(texDesc == TEX_DESC_WIDTH_ODD_HEIGHT_EVEN)
	{
		// get the interpolated value between the 4 pixels
		// sample twice on the uneven axis
		float2 uv0 = texelSize * (globalThreadId + float2(0.25, 0.5));
		float2 uv1 = texelSize * (globalThreadId + float2(0.75, 0.5));
		color = srcMip.SampleLOD(samp, uv0, srcMipLevel); //srcMipLevel
		color += srcMip.SampleLOD(samp, uv1, srcMipLevel);
		color *= .5;
	}

	else if (texDesc == TEX_DESC_WIDTH_EVEN_HEIGHT_ODD)
	{
		// get the interpolated value between the 4 pixels
		// sample twice on the uneven axis
		float2 uv0 = texelSize * (globalThreadId + float2(0.5, 0.25));
		float2 uv1 = texelSize * (globalThreadId + float2(0.5, 0.75));
		color = srcMip.SampleLOD(samp, uv0, srcMipLevel);
		color += srcMip.SampleLOD(samp, uv1, srcMipLevel);
		color *= .5;
	}

	else if (texDesc == TEX_DESC_WIDTH_HEIGHT_ODD)
	{
		// get the interpolated value between the 4 pixels
		// sample twice on the uneven axis
		float2 uv0 = texelSize * (globalThreadId + float2(0.25, 0.5));
		float2 uv1 = texelSize * (globalThreadId + float2(0.75, 0.5));
		float2 uv2 = texelSize * (globalThreadId + float2(0.5, 0.25));
		float2 uv3 = texelSize * (globalThreadId + float2(0.5, 0.75));
		color = srcMip.SampleLOD(samp, uv0, srcMipLevel);
		color += srcMip.SampleLOD(samp, uv1, srcMipLevel);
		color += srcMip.SampleLOD(samp, uv2, srcMipLevel);
		color += srcMip.SampleLOD(samp, uv3, srcMipLevel);
		color *= .25;
	}

	outMip.MipMaps(srcMipLevel, globalThreadId) = color;

	if (numMipLevels == 1)
	return;

	// store all colors of each pixel within the group
	SaveSharedColor(threadInGroup, color);

	// wait for all threads to reach this point
	ThreadGroupMemoryBarrierSync();

	// group = 8 x 8 pixels
	// reducing 8 x 8 to 4 x 4
	// 0x9 = 001001 the first 3 bits are to pass every other x
	// the second 3 bits are to pass every other y
	if((threadInGroup & 0x9) == 0)
	{
		// get the average of the 4 pixels
		float4 color2 = LoadSharedColor(threadInGroup + 1);
		color2 += LoadSharedColor(threadInGroup + 8);
		color2 += LoadSharedColor(threadInGroup + 9);
		color = 0.25 * (color + color2);
		outMip.MipMaps(srcMipLevel + 1, globalThreadId / 2) = GetColor(color);
	}

	if (numMipLevels == 2)
	return;

	ThreadGroupMemoryBarrierSync();

	// 0x9 = 011011
	if((threadInGroup & 0x1B) == 0)
	{
		// get the average of 4 pixels
		float4 color2 = LoadSharedColor(threadInGroup + 0x02);	// 2
		color2 += LoadSharedColor(threadInGroup + 0x10);		// 16
		color2 += LoadSharedColor(threadInGroup + 0x12);		// 18
		color = 0.25 * (color + color2);
		outMip.MipMaps(srcMipLevel + 2, globalThreadId / 4) = GetColor(color);
	}

	if (numMipLevels == 3)
	return;

	ThreadGroupMemoryBarrierSync();

	// 111111, only 0 will be valid
	if (threadInGroup == 0) 
	{
		// get the average of 4 pixels
		float4 color2 = LoadSharedColor(threadInGroup + 0x04);	// 4
		color2 += LoadSharedColor(threadInGroup + 0x20);		// 32
		color2 += LoadSharedColor(threadInGroup + 0x24);		// 36
		color = 0.25 * (color + color2);
		outMip.MipMaps(srcMipLevel + 3, globalThreadId / 8) = GetColor(color);
	}
};								