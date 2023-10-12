#include "..\WVPMatrix.hlsli"

SamplerState samp : register(s0);

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

Texture2D sandTexture : register(t10);
Texture2D grassTexture : register(t11);
Texture2D soilTexture : register(t12);

// �ͷ��� ���� ��ĥ �귯��
cbuffer BrushBuffer : register(b10)
{
    int type;
    float3 pickingPos;
	
    float range;
    float3 color;
}

//struct VertexUVNormalTangent
//{
//    float4 pos : POSITION;
//    float2 uv : UV;
//    float3 normal : NORMAL;
//    float3 tangent : TANGENT;
//};
struct VertexUVNormalTangentAlpha
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};

struct LightPixel
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewPos : POSITION0;
    float3 worldPos : POSITION1;
    float4 alpha : ALPHA;
};

// �귯���� ��ĥ
float4 BrushColor(float3 pos)
{
    float2 direction = pos.xz - pickingPos.xz;
		
    float distance = length(direction);

    if (distance <= range)
        return float4(color, 0);
    
    return float4(0, 0, 0, 0);
}


LightPixel VS(VertexUVNormalTangentAlpha input)
{
    LightPixel output;
    output.pos = mul(input.pos, world);
	
    output.viewPos = invView._41_42_43; // ī�޶��� ���� Position
    output.worldPos = output.pos;       // ���� Position
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;   // �ͷ��� uv
    
    // ���� ����� ���� �븻, ź��Ʈ ���͸� ����
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    // �븻�� ź��Ʈ�� ũ�ν��ϴ� ���̳븻 ��
    output.binormal = cross(output.normal, output.tangent);
    
    output.alpha = input.alpha;
    
    return output;
}

float4 PS(LightPixel input) : SV_Target
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    //float4 albedo = sandTexture.Sample(samp, input.uv);
    float4 sand = sandTexture.Sample(samp, input.uv);
    float4 grass = grassTexture.Sample(samp, input.uv);
    float4 soil = soilTexture.Sample(samp, input.uv);
    
    albedo = lerp(albedo, sand, input.alpha.r);
    albedo = lerp(albedo, grass, input.alpha.g);
    albedo = lerp(albedo, soil, input.alpha.b);
    //float4 specular = specularMap.Sample(samp, input.uv);
    float4 normal = normalMap.Sample(samp, input.uv);
    
    float4 brush = BrushColor(input.worldPos);
    
    return albedo + brush; 
}