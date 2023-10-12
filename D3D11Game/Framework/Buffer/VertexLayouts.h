#pragma once

struct Vertex
{
    Float3 pos = {};

    Vertex(float x, float y, float z)
    {
        pos = { x, y, z };
    }
};

struct VertexColor
{
    Float3 pos = {};
    Float4 color = {};

    VertexColor()
    {
        color = { 1, 1, 1, 1 };
    }

    VertexColor(float x, float y, float z, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
    {
        pos = { x, y, z };
        color = { r, g, b, a };
    }

    VertexColor(float x, float y, float z, Float4 colors = { 1.0f, 1.0f, 1.0f, 1.0f })
    {
        pos = { x, y, z };
        color = { colors.x, colors.y, colors.z, colors.w };
    }
};

struct VertexUV
{
    Float3 pos = {};
    Float2 uv = {};
    VertexUV() {}
    VertexUV(float x, float y, float z, float u, float v)
    {
        pos = { x, y, z };
        uv = { u, v };
    }
};

struct VertexUVNormalTangent
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};     // ǥ�鿡�� �������� ���� ������ ����
    Float3 tangent = {};    // normal�� ���� ����, ���� ���Ͱ� ������ �׷��Ƚ�������
                            // UV�� U ��ǥ�� ��ġ�ϴ� ���͸� Tangent
                            // UV�� V ��ǥ�� ��ġ�ϴ� ���͸� BiTangent �� ���´´�.
    VertexUVNormalTangent() {};

};

struct VertexUVNormalTangentAlpha
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};     // ǥ�鿡�� �������� ���� ������ ����
    Float3 tangent = {};    // normal�� ���� ����, ���� ���Ͱ� ������ �׷��Ƚ�������
    float alpha[4] = {};
    // UV�� U ��ǥ�� ��ġ�ϴ� ���͸� Tangent
    // UV�� V ��ǥ�� ��ġ�ϴ� ���͸� BiTangent �� ���´´�.
    VertexUVNormalTangentAlpha() {};

};

struct VertexUVNormalTangentBlend
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};
    Float3 tangent = {};
    Float4 indices = {};
    Float4 weights = {};

    VertexUVNormalTangentBlend()
    {
    }
};


struct InstanceData
{
    Matrix world = XMMatrixIdentity();
    int index = 0;
};