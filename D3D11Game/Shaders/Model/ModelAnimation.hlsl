#include "../WVPMatrix.hlsli"
#include "../Light/DefaultLight.hlsli"

struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION;          // 포지션
    float2 uv : UV;                 // UV
    float3 normal : NORMAL;         // normal
    float3 tangent : TANGENT;       // tangent
    float4 indices : BLENDINDICES;  // 인덱스
    float4 weights : BLENDWEIGHTS;  // 가중치
};

// vertex
Texture2DArray transformMap : register(t0);

struct Frame
{
    int clip;       // 클립
    uint curFrame;  // 현재 프레임
    float time;     // 시간
    float speed;    // 속도
};

struct Motion
{
    float takeTime;     // 전환에 걸리는 시간
    float tweenTime;    // 전환 시간
    float runningTime;  // 재생 시간
    float padding;
	
    Frame cur;          // 현재 프레임 정보
    Frame next;         // 다음 프레임 정보
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
		
        // 인덱스를 기반으로 행렬 값을 구함
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
		// 애니메이션의 현재 프레임 
        cur = matrix(c0, c1, c2, c3);
        // 인덱스를 기반으로 행렬 값을 구함
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
		// 애니메이션의 다음 프레임
        next = matrix(n0, n1, n2, n3);
		// 현재 애니메이션의 매트릭스
        curAnim = lerp(cur, next, motion.cur.time);
        // 다음 클립
        clip = motion.next.clip;
        curFrame = motion.next.curFrame;
		// 다음 클립이 있을때 행렬 구하기
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
            // 행렬이 있으니 사이값으로 보간
            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }
		// 가중치를 두고 행렬 계산
        transform += mul(weights[i], curAnim);
    }
	
    return transform;
}

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
    LightPixelInput output;
    // 해당 정점에 대한 가중치를 기준으로 월드 행렬, 트랜스폼 계산
    matrix transform = mul(SkinWorld(input.indices, input.weights), world);
    output.pos = mul(input.pos, transform); // 위치값 계산
    //output.pos = mul(input.pos, world);
	
    output.viewPos = invView._41_42_43; // 역행렬에서 카메라 뷰 포지션 가져오기
    output.worldPos = output.pos;       // 대상 포지션
	
    output.pos = mul(output.pos, view); // 뷰 행렬 곱하기
    output.pos = mul(output.pos, projection);   // 프로젝션 행렬 곱하기

    output.uv = input.uv;   // uv값 전달
	// 계산 한 트랜스폼을 기준으로 normal, tangent, binormal 값 구하기
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