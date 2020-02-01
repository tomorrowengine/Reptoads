cbuffer MatCB : register(b0)
{
    matrix OrthoViewMat;
}

struct VertexShaderInput{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct VertexShaderOutput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};


VertexShaderOutput main(VertexShaderInput IN)
{
    VertexShaderOutput OUT;

    matrix MVPMatrix = OrthoViewMat;
    OUT.Position = mul(MVPMatrix, float4(IN.Position, 1.0f));
    //OUT.Position = float4(IN.Position, 1.0);
    OUT.TexCoord = IN.TexCoord;
 
    return OUT;
}
