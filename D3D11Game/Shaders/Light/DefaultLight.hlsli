//Pixel Struct
// Default Light
struct DefaultLight
{
    float4 color;       // ���� ����
	
    float3 direction;   // ���� ����
    float intensity;    // ���� ����
};

// ����
struct Material
{
    float3 normal;              // ���� ���� ����
    float4 diffuseColor;        // �⺻ ����
    float4 specularIntensity;   // ���ݻ� ����
    float3 viewPos;             // ī�޶� ��ġ
    float3 worldPos;            // ��� ��ġ
};

// ���� ����
struct LightPixelInput
{
    float4 pos : SV_POSITION;   // ������
    float2 uv : UV;             // uv ����
    float3 normal : NORMAL;     // �븻 ����
    float3 tangent : TANGENT;   // ź��Ʈ ����
    float3 binormal : BINORMAL; // biź��Ʈ ����
    float3 viewPos : POSITION0; // ī�޶� ��ġ
    float3 worldPos : POSITION1;// ��� ��ġ
};

// Buffer
//Pixel ConstBuffer
cbuffer LightBuffer : register(b0)
{
    DefaultLight light;
}
// ���׸��� ����
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
// �븻 �� ���
float3 NormalMapping(float3 T, float3 B, float3 N, float2 uv)
{
    // tangent, binormal, normal
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
	
	[flatten]
    if (hasNormalMap)   // �븻���� ���� ���
    {
        float3 normal = normalMap.Sample(samp, uv).rgb;
        normal = normal * 2.0f - 1.0f; // 0~1 -> -1~1
        float3x3 TBN = float3x3(T, B, N);
        N = normalize(mul(normal, TBN));
    }
	
    return N;
}
// ���׸��� ����
Material GetMaterial(LightPixelInput input)
{
    Material material;
    material.normal = NormalMapping(input.tangent,
		input.binormal, input.normal, input.uv);    // normal ���
		
    material.diffuseColor = diffuseMap.Sample(samp, input.uv);  // �⺻ �÷�
    material.specularIntensity = specularMap.Sample(samp, input.uv);    // ���ݻ� ��
    material.viewPos = input.viewPos;
    material.worldPos = input.worldPos;
	
    return material;
}

float4 CalcDirectional(Material material, DefaultLight light)
{
    // ����Ʈ-�Ǿ ���� ��, ���� ���̵� ����
    float3 toLight = normalize(light.direction);    // ���� ���� ����ȭ
	
    // ���� ���Ϳ� ǥ�� ���� ������ ���� ���� ���ϰ�, �̸� 0�� 1 ������ ������ Ŭ�����Ͽ� ǥ���� ��⸦ ����
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
	
	[flatten]
    if (diffuseIntensity > 0)
    {
        // ī�޶� ��ġ���� ��� ��ġ�� ���� ����
        float3 viewDir = normalize(material.worldPos - material.viewPos);
        // �ݻ� ���� ����
        float3 halfWay = normalize(viewDir + toLight);
        // ǥ��� �ݻ� ���� ���͸� Ŭ����
        float specular = saturate(dot(material.normal, -halfWay));
		// �ݻ籤 ������
        finalColor += light.color * pow(specular, shininess)
			* material.specularIntensity * mSpecular;
    }
	
    return finalColor * material.diffuseColor;
}
// �ֺ���
float4 CalcAmbient(Material material)
{
    float up = material.normal.y * 0.5f + 0.5f; // -1~1 -> 0~1
	// �ֺ��� ���
    //float4 resultAmbient = float4(ambientLight + up * ambientCeil, 1.0f);
    float4 resultAmbient = float4(float3(0.3f, 0.3f, 0.3f) + up * float3(0.3f, 0.3f, 0.3f), 1.0f);
	// �ֺ��� �߰�
    return resultAmbient * material.diffuseColor * mAmbient;
}
// �߱�
float4 CalcEmissive(Material material)
{
    float emissiveIntensity = 0.0f;
	
	[flatten]
    if (mEmissive.a > 0.0f)
    {
        // ī�޶� ��ġ���� ��� ��ġ�� ���� ����
        float3 viewDir = normalize(material.viewPos - material.worldPos);
        // �븻 ���� ����
        float t = saturate(dot(material.normal, viewDir));
        // Emissive���� ����
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