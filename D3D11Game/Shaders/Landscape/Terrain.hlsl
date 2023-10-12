#include "..\WVPMatrix.hlsli"
#include "..\Light\DefaultLight.hlsli"

//SamplerState samp : register(s0);
//
//Texture2D diffuseMap : register(t0);
//Texture2D specularMap : register(t1);
//Texture2D normalMap : register(t2);

//Texture2D depthTexture : register(t9);
Texture2D alphaTexture : register(t10);
Texture2D sandTexture : register(t11);
Texture2D grassTexture : register(t12);
Texture2D soilTexture : register(t13);

struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

//struct LightPixel
//{
//    float4 pos : SV_POSITION;
//    float2 uv : UV;
//    float3 normal : NORMAL;
//    float3 tangent : TANGENT;
//    float3 binormal : BINORMAL;
//    float3 viewPos : POSITION0;
//    float3 worldPos : POSITION1;
//    float4 alpha : ALPHA;
//};

LightPixelInput VS(VertexUVNormalTangent input)
{
    LightPixelInput output;
    output.pos = mul(input.pos, world);
	
    output.viewPos = invView._41_42_43; // ī�޶��� ���� Position
    output.worldPos = output.pos; // ���� Position
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv; // �ͷ��� uv
    
    // ���� ����� ���� �븻, ź��Ʈ ���͸� ����
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    // �븻�� ź��Ʈ�� ũ�ν��ϴ� ���̳븻 ��
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}

float4 PS(LightPixelInput input) : SV_Target
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    float4 sand = sandTexture.Sample(samp, input.uv);
    float4 grass = grassTexture.Sample(samp, input.uv);
    float4 soil = soilTexture.Sample(samp, input.uv);
    //float4 depth = depthTexture.Sample(samp, input.uv * 0.1f);
    
    float4 alpha = alphaTexture.Sample(samp, input.uv * 0.1f);
    
    albedo = lerp(albedo, sand, alpha.r);
    albedo = lerp(albedo, grass, alpha.g);
    albedo = lerp(albedo, soil, alpha.b);
    //albedo = lerp(albedo, depth, 0.5f);
    
    Material material = GetMaterial(input);
    material.diffuseColor = albedo;
	
    float4 color = CalcDirectional(material, light);
    float4 ambient = CalcAmbient(material);
    float4 emissive = CalcEmissive(material);
    
    return color + ambient + emissive;
}