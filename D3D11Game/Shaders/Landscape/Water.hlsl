#include "..\WVPMatrix.hlsli"
#include "..\Light\DefaultLight.hlsli"

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

cbuffer WaterBuffer : register(b10)
{
    float2 tile;
    float2 moveScale;

    float timeScale;
    float waveScale;
    float alpha;
    float padding;
}

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    float2 uv = input.uv * 0.1f + moveScale * 0.1f;
    float4 normal = normalMap.Sample(samp, uv) * 2.0f - 1.0f;
    
    float2 uv2 = normal.xy;
    float3 lightDir = normalize(light.direction);
    float intensity = saturate(dot(normal, -lightDir));

    input.uv *= tile;
    input.uv += moveScale * timeScale;
    float4 color = diffuseMap.Sample(samp, input.uv);
    color *= diffuseMap.Sample(samp, uv2);
    color *= 1.5f;
    color.a = alpha;
    return color;

}