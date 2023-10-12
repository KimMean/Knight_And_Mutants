#include "..\WVPMatrix.hlsli"

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};
struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 originPos : POSITION;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    // ���� ��ǥ�� ��� ����
    output.pos.xyz = mul(input.pos.xyz, (float3x3) view);
    output.pos.w = 1.0f;    // ���� ó����
    output.pos = mul(output.pos, projection);   // ��������
    
    output.originPos = input.pos.xyz;
    
    return output;
}

// ���÷�������Ʈ 
SamplerState samp : register(s0);
TextureCube cubeMap : register(t10);

float4 PS(PixelInput input) : SV_TARGET
{
    return float4(cubeMap.Sample(samp, input.originPos).rgb, 1.0f);
}