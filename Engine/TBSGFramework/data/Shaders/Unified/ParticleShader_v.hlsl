#include "ParticleShaderBuffers.h"

struct VertexShaderInput {
	V3 Position : POSITION;
	V2 TexCoord : TEXCOORD;
};

VertexShaderOutput main(VertexShaderInput IN)
{
    VertexShaderOutput OUT;

    OUT.Position = mul(MVPMatrix, float4(IN.Position, 1.0f));
    OUT.TexCoord = IN.TexCoord;
 
    return OUT;
}
