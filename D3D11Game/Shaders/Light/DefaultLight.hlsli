//Pixel Struct
// Default Light
struct DefaultLight
{
    float4 color;       // 빛의 색상
	
    float3 direction;   // 빛의 방향
    float intensity;    // 빛의 강도
};

// 재질
struct Material
{
    float3 normal;              // 면의 수직 방향
    float4 diffuseColor;        // 기본 색상
    float4 specularIntensity;   // 정반사 강도
    float3 viewPos;             // 카메라 위치
    float3 worldPos;            // 대상 위치
};

// 받을 정보
struct LightPixelInput
{
    float4 pos : SV_POSITION;   // 포지션
    float2 uv : UV;             // uv 정보
    float3 normal : NORMAL;     // 노말 벡터
    float3 tangent : TANGENT;   // 탄젠트 벡터
    float3 binormal : BINORMAL; // bi탄젠트 벡터
    float3 viewPos : POSITION0; // 카메라 위치
    float3 worldPos : POSITION1;// 대상 위치
};

// Buffer
//Pixel ConstBuffer
cbuffer LightBuffer : register(b0)
{
    DefaultLight light;
}
// 메테리얼 정보
cbuffer MaterialBuffer : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float4 mEmissive;
	
    float shininess;
    int hasNormalMap;
}

// Sampler
SamplerState samp : register(s0);

// Texture
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

//Pixel Function
// 노말 값 계산
float3 NormalMapping(float3 T, float3 B, float3 N, float2 uv)
{
    // tangent, binormal, normal
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
	
	[flatten]
    if (hasNormalMap)   // 노말맵이 있을 경우
    {
        float3 normal = normalMap.Sample(samp, uv).rgb;
        normal = normal * 2.0f - 1.0f; // 0~1 -> -1~1
        float3x3 TBN = float3x3(T, B, N);
        N = normalize(mul(normal, TBN));
    }
	
    return N;
}
// 메테리얼 추출
Material GetMaterial(LightPixelInput input)
{
    Material material;
    material.normal = NormalMapping(input.tangent,
		input.binormal, input.normal, input.uv);    // normal 계산
		
    material.diffuseColor = diffuseMap.Sample(samp, input.uv);  // 기본 컬러
    material.specularIntensity = specularMap.Sample(samp, input.uv);    // 정반사 값
    material.viewPos = input.viewPos;
    material.worldPos = input.worldPos;
	
    return material;
}

float4 CalcDirectional(Material material, DefaultLight light)
{
    // 램버트-피어스 조명 모델, 블린퐁 쉐이딩 참조
    float3 toLight = normalize(light.direction);    // 빛의 방향 정규화
	
    // 조명 벡터와 표면 법선 벡터의 내적 값을 구하고, 이를 0과 1 사이의 값으로 클램핑하여 표면의 밝기를 결정
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
	
	[flatten]
    if (diffuseIntensity > 0)
    {
        // 카메라 위치에서 대상 위치로 가는 벡터
        float3 viewDir = normalize(material.worldPos - material.viewPos);
        // 반사 방향 벡터
        float3 halfWay = normalize(viewDir + toLight);
        // 표면과 반사 방향 벡터를 클램핑
        float specular = saturate(dot(material.normal, -halfWay));
		// 반사광 더해줌
        finalColor += light.color * pow(specular, shininess)
			* material.specularIntensity * mSpecular;
    }
	
    return finalColor * material.diffuseColor;
}
// 주변광
float4 CalcAmbient(Material material)
{
    float up = material.normal.y * 0.5f + 0.5f; // -1~1 -> 0~1
	// 주변광 계산
    //float4 resultAmbient = float4(ambientLight + up * ambientCeil, 1.0f);
    float4 resultAmbient = float4(float3(0.3f, 0.3f, 0.3f) + up * float3(0.3f, 0.3f, 0.3f), 1.0f);
	// 주변광 추가
    return resultAmbient * material.diffuseColor * mAmbient;
}
// 발광
float4 CalcEmissive(Material material)
{
    float emissiveIntensity = 0.0f;
	
	[flatten]
    if (mEmissive.a > 0.0f)
    {
        // 카메라 위치에서 대상 위치로 가는 벡터
        float3 viewDir = normalize(material.viewPos - material.worldPos);
        // 노말 값과 내적
        float t = saturate(dot(material.normal, viewDir));
        // Emissive강도 설정
        emissiveIntensity = smoothstep(1.0f - mEmissive.a, 1.0f, 1.0f - t);
    }
	
    return mEmissive * emissiveIntensity;
}

float4 CalcLights(LightPixelInput input)
{
    Material material = GetMaterial(input);
	
    float4 color = CalcDirectional(material, light);
    float4 ambient = CalcAmbient(material);
    float4 emissive = CalcEmissive(material);
    
    return color + ambient + emissive;
}