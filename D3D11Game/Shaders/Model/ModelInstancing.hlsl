#include "../WVPMatrix.hlsli"
#include "../Light/DefaultLight.hlsli"

struct ModelInstanceData
{
    float4 pos : POSITION; // 포지션
    float2 uv : UV; // UV
    float3 normal : NORMAL; // normal
    float3 tangent : TANGENT; // tangent
    float4 indices : BLENDINDICES; // 인덱스
    float4 weights : BLENDWEIGHTS; // 가중치
    
    matrix transform : INSTANCE_TRANSFORM;
};

LightPixelInput VS(ModelInstanceData input)
{
    LightPixelInput output;
    output.pos = mul(input.pos, input.transform);
	
    output.viewPos = invView._41_42_43;
    output.worldPos = output.pos;
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
	
    output.normal = mul(input.normal, (float3x3) input.transform);
    output.tangent = mul(input.tangent, (float3x3) input.transform);
    output.binormal = cross(output.normal, output.tangent);
	
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    return CalcLights(input);
}