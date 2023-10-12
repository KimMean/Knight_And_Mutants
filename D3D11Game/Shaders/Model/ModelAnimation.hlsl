#include "../WVPMatrix.hlsli"
#include "../Light/DefaultLight.hlsli"

struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION;          // ������
    float2 uv : UV;                 // UV
    float3 normal : NORMAL;         // normal
    float3 tangent : TANGENT;       // tangent
    float4 indices : BLENDINDICES;  // �ε���
    float4 weights : BLENDWEIGHTS;  // ����ġ
};

// vertex
Texture2DArray transformMap : register(t0);

struct Frame
{
    int clip;       // Ŭ��
    uint curFrame;  // ���� ������
    float time;     // �ð�
    float speed;    // �ӵ�
};

struct Motion
{
    float takeTime;     // ��ȯ�� �ɸ��� �ð�
    float tweenTime;    // ��ȯ �ð�
    float runningTime;  // ��� �ð�
    float padding;
	
    Frame cur;          // ���� ������ ����
    Frame next;         // ���� ������ ����
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

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
    LightPixelInput output;
    // �ش� ������ ���� ����ġ�� �������� ���� ���, Ʈ������ ���
    matrix transform = mul(SkinWorld(input.indices, input.weights), world);
    output.pos = mul(input.pos, transform); // ��ġ�� ���
    //output.pos = mul(input.pos, world);
	
    output.viewPos = invView._41_42_43; // ����Ŀ��� ī�޶� �� ������ ��������
    output.worldPos = output.pos;       // ��� ������
	
    output.pos = mul(output.pos, view); // �� ��� ���ϱ�
    output.pos = mul(output.pos, projection);   // �������� ��� ���ϱ�

    output.uv = input.uv;   // uv�� ����
	// ��� �� Ʈ�������� �������� normal, tangent, binormal �� ���ϱ�
    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);
	
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    //Material material = GetMaterial(input);
	//
    //float4 color = CalcDirectional(material, light);
    //float4 ambient = CalcAmbient(material);
    //float4 emissive = CalcEmissive(material);
	//
    //return color + ambient + emissive;
    return CalcLights(input);

}