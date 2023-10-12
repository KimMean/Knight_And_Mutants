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

// VS에서 처리 후 반환
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
    
    // 이를 활용하여 카메라가 대상을 바라보는 방향을 구할 수 있으며
    // 빛 계산에 잘 활용될 예정
    output.viewPos = invView._41_42_43; // 카메라의 위치
    output.worldPos = output.pos;       // 대상의 위치
    
    // 월드 포지션에 뷰행렬, 프로젝션 행렬을 곱해 2차원 좌표로
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
	
    // 입력받은 normal, tangent벡터는 로컬 벡터임
    // 월드행렬과 계산을 해줘야 월드상의 벡터가 나옴
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
	
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    // 반사율
    float4 albedo = diffuseMap.Sample(samp, input.uv);
	
    // TBN을 정규화 함
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
	
    // 빛의 방향 정규화
    float3 lightDir = normalize(light.direction);
    // 노말벡터와 빛의 방향을 내적하면 강도가 나옴
    float intensity = saturate(dot(N, -lightDir));
    // 텍스처를 샘플러스테이트에 저장된 형식으로 읽어드림
    // 빛의 강도를 곱함, 반환값은 float4 컬러값
    
    return diffuseMap.Sample(samp, input.uv) * intensity;
}