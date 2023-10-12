#include "../WVPMatrix.hlsli"
#include "../Light/DefaultLight.hlsli"

#define MAX_INSTANCE 128

struct ModelInstanceData
{
    float4 pos : POSITION; // 포지션
    float2 uv : UV; // UV
    float3 normal : NORMAL; // normal
    float3 tangent : TANGENT; // tangent
    float4 indices : BLENDINDICES; // 인덱스
    float4 weights : BLENDWEIGHTS; // 가중치
    
    matrix transform : INSTANCE_TRANSFORM;
    int index : INSTANCE_INDEX;
};

struct Frame
{
    int clip;
    uint curFrame;
    float time;
    float speed;
};

struct Motion
{
    float takeTime;
    float tweenTime;
    float runningTime;
    float padding;
	
    Frame cur;
    Frame next;
};

cbuffer FrameInstancingBuffer : register(b4)
{
    Motion motions[MAX_INSTANCE];
}


Texture2DArray transformMap : register(t0);

matrix SkinWorld(int instanceID, float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix cur, next;
    matrix curAnim, nextAnim;
	
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
	
    for (int i = 0; i < 4; i++)
    {
        int clip = motions[instanceID].cur.clip;
        int curFrame = motions[instanceID].cur.curFrame;
		
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
		
        curAnim = lerp(cur, next, motions[instanceID].cur.time);

        clip = motions[instanceID].next.clip;
        curFrame = motions[instanceID].next.curFrame;
		
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
			
            nextAnim = lerp(cur, next, motions[instanceID].next.time);

            curAnim = lerp(curAnim, nextAnim, motions[instanceID].tweenTime);
        }
		
        transform += mul(weights[i], curAnim);
    }
	
    return transform;
}

LightPixelInput VS(ModelInstanceData input)
{
    LightPixelInput output;
    matrix transform = mul(SkinWorld(input.index, input.indices, input.weights), input.transform);
	
    output.pos = mul(input.pos, transform);
	
    output.viewPos = invView._41_42_43;
    output.worldPos = output.pos;
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
	
    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);
	
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    return CalcLights(input);
}