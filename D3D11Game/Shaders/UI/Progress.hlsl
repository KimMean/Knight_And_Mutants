#include "..\WVPMatrix.hlsli"

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};

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

SamplerState samp : register(s0);
Texture2D diffuseMap : register(t0);

cbuffer ValueBuffer : register(b10)
{
    float fillAmount;
}

float4 PS(PixelInput input) : SV_TARGET
{
    if (input.uv.x < fillAmount)
        return diffuseMap.Sample(samp, input.uv);

    return float4(0.0f, 0.0f, 0.0f, 0.0f);

}