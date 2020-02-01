float4 main(float3 pos : POSITION) : SV_Position
{
    return float4(pos, 1.0f);
}
