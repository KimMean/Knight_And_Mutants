#include "..\WVPMatrix.hlsli"

struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct PixelInput
{
    float4 pos : SV_Position;
};

PixelInput VS(VertexUVNormalTangent input)
{
    PixelInput output;
	
    matrix transform;
	
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float depth = input.pos.z / input.pos.w;
	
    return float4(depth.xxx, 1);
}