//Pixel Struct
struct DirectLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct LightPixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewPos : POSITION0;
    float3 worldPos : POSITION1;
};

// Sampler
SamplerState samp : register(s0);

// Texture
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

cbuffer LightBuffer : register(b0)
{
    DirectLight light;
}