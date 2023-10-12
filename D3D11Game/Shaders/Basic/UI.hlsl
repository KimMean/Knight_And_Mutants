#include "..\WVPMatrix.hlsli"

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};

SamplerState samp : register(s0);
Texture2D diffuseMap : register(t0);

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
	
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return diffuseMap.Sample(samp, input.uv);
}