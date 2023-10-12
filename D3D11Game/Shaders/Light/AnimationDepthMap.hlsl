#include "..\WVPMatrix.hlsli"
//#include "..\Light\DefaultLight.hlsli"

struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION; // 포지션
    float2 uv : UV; // UV
    float3 normal : NORMAL; // normal
    float3 tangent : TANGENT; // tangent
    float4 indices : BLENDINDICES; // 인덱스
    float4 weights : BLENDWEIGHTS; // 가중치
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

cbuffer FrameBuffer : register(b3)
{
    Motion motion;
}
Texture2DArray transformMap : register(t0);

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
		
        curAnim = lerp(cur, next, motion.cur.time);

        clip = motion.next.clip;
        curFrame = motion.next.curFrame;
		
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

            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }
		
        transform += mul(weights[i], curAnim);
    }
	
    return transform;
}

struct PixelInput
{
    float4 pos : SV_Position;
};

PixelInput VS(VertexUVNormalTangentBlend input)
{
    PixelInput output;
	
    matrix transform;
	
    transform = mul(SkinWorld(input.indices, input.weights), world);
	
    output.pos = mul(input.pos, transform);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float depth = input.pos.z / input.pos.w;
	
    return float4(depth.xxx, 1);
}

