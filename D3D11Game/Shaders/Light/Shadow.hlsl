#include "..\WVPMatrix.hlsli"
#include "../Light/DefaultLight.hlsli"
// VertexLayout

struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION; // ������
    float2 uv : UV; // UV
    float3 normal : NORMAL; // normal
    float3 tangent : TANGENT; // tangent
    float4 indices : BLENDINDICES; // �ε���
    float4 weights : BLENDWEIGHTS; // ����ġ
};

// vertex
Texture2DArray transformMap : register(t0);

struct Frame
{
    int clip; // Ŭ��
    uint curFrame; // ���� ������
    float time; // �ð�
    float speed; // �ӵ�
};

struct Motion
{
    float takeTime; // ��ȯ�� �ɸ��� �ð�
    float tweenTime; // ��ȯ �ð�
    float runningTime; // ��� �ð�
    float padding;
	
    Frame cur; // ���� ������ ����
    Frame next; // ���� ������ ����
};

cbuffer FrameBuffer : register(b3)
{
    Motion motion;
}

//Vertex Function
matrix SkinWorld(float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix cur, next;
    matrix curAnim, nextAnim;
	
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
	
    for (int i = 0; i < 4; i++)
    {
        int clip = motion.cur.clip;
        int curFrame = motion.cur.curFrame;
		
        // �ε����� ������� ��� ���� ����
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
		// �ִϸ��̼��� ���� ������ 
        cur = matrix(c0, c1, c2, c3);
        // �ε����� ������� ��� ���� ����
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
		// �ִϸ��̼��� ���� ������
        next = matrix(n0, n1, n2, n3);
		// ���� �ִϸ��̼��� ��Ʈ����
        curAnim = lerp(cur, next, motion.cur.time);
        // ���� Ŭ��
        clip = motion.next.clip;
        curFrame = motion.next.curFrame;
		// ���� Ŭ���� ������ ��� ���ϱ�
        if (clip > -1)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
		
            cur = matrix(c0, c1, c2, c3);
		
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
		
            next = matrix(n0, n1, n2, n3);
			
            nextAnim = lerp(cur, next, motion.next.time);
            // ����� ������ ���̰����� ����
            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }
		// ����ġ�� �ΰ� ��� ���
        transform += mul(weights[i], curAnim);
    }
	
    return transform;
}

struct PixelInput
{
    float4 pos : SV_POSITION;       // ������
    float2 uv : UV;                 // uv ����
    float3 normal : NORMAL;         // �븻 ����
    float3 tangent : TANGENT;       // ź��Ʈ ����
    float3 binormal : BINORMAL;     // biź��Ʈ ����
    float3 viewPos : POSITION0;     // ī�޶� ��ġ
    float3 worldPos : POSITION1;    // ��� ��ġ
    float4 clipPos : POSITION2;     // ������ �ٶ� ��� ��ġ
};

Texture2D depthMap : register(t9);

cbuffer LightView : register(b11)
{
    matrix lightView;
}

cbuffer LightProjection : register(b12)
{
    matrix lightProjection;
}



PixelInput VS(VertexUVNormalTangentBlend input)
{
    PixelInput output;
    // �ش� ������ ���� ����ġ�� �������� ���� ���, Ʈ������ ���
    matrix transform = mul(SkinWorld(input.indices, input.weights), world);
    output.pos = mul(input.pos, transform); // ��ġ�� ���
	
    output.viewPos = invView._41_42_43;
    output.worldPos = output.pos;
	// ���� ����� ����
    output.clipPos = mul(output.pos, lightView);
    output.clipPos = mul(output.clipPos, lightProjection);
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
	
    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);
	
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    Material material;
    material.normal = NormalMapping(input.tangent, input.binormal, input.normal, input.uv);
    material.diffuseColor = diffuseMap.Sample(samp, input.uv);
    material.specularIntensity = specularMap.Sample(samp, input.uv);
    material.viewPos = input.viewPos;
    material.worldPos = input.worldPos;
	
    float4 color = 0;
    
    color += CalcDirectional(material, light);
	//[unroll(MAX_LIGHT)]
    //for (int i = 0; i < lightCount; i++)
    //{
	//	[flatten]
    //    if (!lights[i].active)
    //        continue;
    //
	//	[flatten]
    //    if (lights[i].type == 0)
    //        color += CalcDirectional(material, lights[i]);
    //    else if (lights[i].type == 1)
    //        color += CalcPoint(material, lights[i]);
    //    else if (lights[i].type == 2)
    //        color += CalcSpot(material, lights[i]);
    //    else if (lights[i].type == 3)
    //        color += CalcCapsule(material, lights[i]);
    //}
	
    float4 ambient = CalcAmbient(material);
    float4 emissive = mEmissive;
	
    color = color + ambient + emissive;
	
    // 2d ��ǥ�� ��ȯ
    float2 uv = input.clipPos.xy / input.clipPos.w;
    uv.y = -uv.y;
    uv = uv * 0.5f + 0.5f;
	// x,y�� ������ �ʰ��ϸ� �׳� �÷��� ���
    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
        return color;

    float shadowDepth = depthMap.Sample(samp, uv).r;

    float currentDepth = input.clipPos.z / input.clipPos.w;
	
    if (currentDepth > shadowDepth + 0.0001f)
        return color * 0.5f;

    return color;
}