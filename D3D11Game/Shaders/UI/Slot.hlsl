#include "..\WVPMatrix.hlsli"

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};

//float4 borderColor = { 1.0f, 0.458823f, 0.196078f, 1.0f };
//float4 borderColor = float4(1.0f, 0.4f, 0.2f, 1.0f);
//float borderThickness = 0.5f;

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
    float4 color = diffuseMap.Sample(samp, input.uv);
    ////텍스처 경계선에서 얼마나 떨어져 있는지 계산
    //float2 borderDistance = abs(input.uv - 0.5f) * 2.0f;    // 0 ~ 1
    ////테두리 색상 계산
    //float4 border = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f), float4(1.0f, 0.4f, 0.2f, 1.0f), 
    //                saturate((length(borderDistance)) - 0.9f / 0.9f));
    //
    ////최종 색상 계산
    //float4 pixelColor = lerp(color, border, step(length(borderDistance), 0.95f));
    ////color = color + lineColor;
    return color;

}