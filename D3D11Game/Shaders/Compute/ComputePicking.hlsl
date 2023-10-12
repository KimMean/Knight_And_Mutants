// ���� ����
cbuffer RayBuffer : register(b0)
{
    float3 pos;
    uint triangleSize;
	
    float3 dir;
}

// �ﰢ�� ���� ����
struct InputDesc
{
    float3 v0, v1, v2;
};
StructuredBuffer<InputDesc> input : register(t0);

// �ﰢ�� ���ο� ����� �ε����� �Ÿ�
struct OutputDesc
{
    int picked;
    float distance;
};
RWStructuredBuffer<OutputDesc> output : register(u0);

void Intersection(uint index)
{
    // ���� 3�� �ް�
    float3 A = input[index].v0;
    float3 B = input[index].v1;
    float3 C = input[index].v2;
	
    // 0�� �������� 1, 2������ ���� �Ÿ��� ���Ѵ�.
    float3 e0 = B - A;
    float3 e1 = C - A;
	
    // ������ ������ �� �̿��ؼ� PTQ�� ���ϰ� u, v ���� ����
    float3 P, T, Q;
    P = cross(dir, e1);
	
    float d = 1.0f / dot(e0, P);

    float u, v;
	
    T = pos - A;
    u = dot(T, P) * d;
	
    Q = cross(T, e0);
    v = dot(dir, Q) * d;
    output[index].distance = dot(e1, Q) * d;

    // ���������ؼ� ���̿� �浹�Ǵ��� �˻��Ѵ�.
    bool b = (u >= 0.0f) && (v >= 0.0f) &&
				(u + v <= 1.0f) && (output[index].distance >= 0.0f);

    // �浹�Ǹ� true, �ƴϸ� false
    output[index].picked = b ? 1 : 0;
}

// ����ǰ� �ִ� ������ ���̵� ����
[numthreads(64, 1, 1)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x;
	
    if (triangleSize > index)
        Intersection(index);
}