#include "ParticleShaderBuffers.h"

struct PSOut
{
	float4 diffuse : SEM_TARGET_OUTPUT_(0);
	float4 emissive : SEM_TARGET_OUTPUT_(1);
};

Texture2D ParticleTexture : register(t0);
Texture2D EmissiveTexture : register(t1);
SamplerState TextureSampler : register(s0);

PSOut main(VertexShaderOutput IN ) : SEM_TARGET_OUTPUT
{
	PSOut outValues;

	outValues.diffuse = ParticleTexture.Sample(TextureSampler, IN.TexCoord);
	outValues.emissive = EmissiveTexture.Sample(TextureSampler, IN.TexCoord);

    return outValues;
}
