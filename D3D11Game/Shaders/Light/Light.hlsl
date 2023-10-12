#include "..\WVPMatrix.hlsli"

// VertexLayout
struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

//Pixel Struct
struct DirectLight
{
    float4 color;
    float3 direction;
    float intensity;
};

// VS���� ó�� �� ��ȯ
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

struct Material
{
    float3 normal;
    float4 diffuseColor;
    float4 specularIntensity;
    float3 viewPos;
    float3 worldPos;
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


LightPixelInput VS(VertexUVNormalTangent input)
{
    LightPixelInput output;
    output.pos = mul(input.pos, world);
    
    // �̸� Ȱ���Ͽ� ī�޶� ����� �ٶ󺸴� ������ ���� �� ������
    // �� ��꿡 �� Ȱ��� ����
    output.viewPos = invView._41_42_43; // ī�޶��� ��ġ
    output.worldPos = output.pos;       // ����� ��ġ
    
    // ���� �����ǿ� �����, �������� ����� ���� 2���� ��ǥ��
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
	
    // �Է¹��� normal, tangent���ʹ� ���� ������
    // ������İ� ����� ����� ������� ���Ͱ� ����
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
	
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    // �ݻ���
    float4 albedo = diffuseMap.Sample(samp, input.uv);
	
    // TBN�� ����ȭ ��
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
	
    // ���� ���� ����ȭ
    float3 lightDir = normalize(light.direction);
    // �븻���Ϳ� ���� ������ �����ϸ� ������ ����
    float intensity = saturate(dot(N, -lightDir));
    // �ؽ�ó�� ���÷�������Ʈ�� ����� �������� �о�帲
    // ���� ������ ����, ��ȯ���� float4 �÷���
    
    return diffuseMap.Sample(samp, input.uv) * intensity;
}