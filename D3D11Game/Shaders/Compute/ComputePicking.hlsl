// 레이 정보
cbuffer RayBuffer : register(b0)
{
    float3 pos;
    uint triangleSize;
	
    float3 dir;
}

// 삼각형 정점 정보
struct InputDesc
{
    float3 v0, v1, v2;
};
StructuredBuffer<InputDesc> input : register(t0);

// 삼각형 내부에 검출된 인덱스와 거리
struct OutputDesc
{
    int picked;
    float distance;
};
RWStructuredBuffer<OutputDesc> output : register(u0);

void Intersection(uint index)
{
    // 정점 3개 받고
    float3 A = input[index].v0;
    float3 B = input[index].v1;
    float3 C = input[index].v2;
	
    // 0번 정점에서 1, 2번으로 가는 거리를 구한다.
    float3 e0 = B - A;
    float3 e1 = C - A;
	
    // 외적과 내적을 잘 이용해서 PTQ를 구하고 u, v 값을 구함
    float3 P, T, Q;
    P = cross(dir, e1);
	
    float d = 1.0f / dot(e0, P);

    float u, v;
	
    T = pos - A;
    u = dot(T, P) * d;
	
    Q = cross(T, e0);
    v = dot(dir, Q) * d;
    output[index].distance = dot(e1, Q) * d;

    // 어찌저찌해서 레이에 충돌되는지 검사한다.
    bool b = (u >= 0.0f) && (v >= 0.0f) &&
				(u + v <= 1.0f) && (output[index].distance >= 0.0f);

    // 충돌되면 true, 아니면 false
    output[index].picked = b ? 1 : 0;
}

// 실행되고 있는 스레드 아이디를 받음
[numthreads(64, 1, 1)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x;
	
    if (triangleSize > index)
        Intersection(index);
}