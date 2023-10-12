#pragma once

class Sphere
{
private:
    //typedef VertexUV VertexType;
    typedef VertexUVNormalTangent VertexType;

public:
    Sphere(float radius = 1.0f, UINT sliceCount = 32, UINT stackCount = 16);
    ~Sphere();

    void Render();

private:
    void MakeMesh();
    void MakeTangent();

public :
    GameObject* gameObject;

private:
    Mesh<VertexType>* mesh;

    float radius;
    UINT sliceCount, stackCount;
};