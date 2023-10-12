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
    Float3 normal = {};     // 표면에서 수직으로 뻗어 나오는 벡터
    Float3 tangent = {};    // normal의 수직 벡터, 수직 벡터가 많지만 그래픽스에서는
                            // UV중 U 좌표와 일치하는 벡터를 Tangent
                            // UV중 V 좌표와 일치하는 벡터를 BiTangent 라 일컫는다.
    VertexUVNormalTangent() {};

};

struct VertexUVNormalTangentAlpha
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};     // 표면에서 수직으로 뻗어 나오는 벡터
    Float3 tangent = {};    // normal의 수직 벡터, 수직 벡터가 많지만 그래픽스에서는
    float alpha[4] = {};
    // UV중 U 좌표와 일치하는 벡터를 Tangent
    // UV중 V 좌표와 일치하는 벡터를 BiTangent 라 일컫는다.
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