SamplerState samp : register(s0);
Texture2D diffuseMap : register(t0);

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

float4 PS(PixelInput input) : SV_TARGET
{
    return diffuseMap.Sample(samp, input.uv);
}