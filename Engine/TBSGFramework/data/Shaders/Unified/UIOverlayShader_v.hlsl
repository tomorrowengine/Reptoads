#include "UIOverlayShaderBuffers.h"

struct VertexShaderInput
{
	float3 Position : S_POSITION;
	float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT main(VertexShaderInput IN)
{
	VS_OUTPUT OUT;

    OUT.Position = mul(projectionMatrix, float4(IN.Position, 1.0f));
    OUT.UV = IN.TexCoord;
 
    return OUT;
}
