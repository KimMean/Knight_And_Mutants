#pragma once

class Cube
{
private:
    typedef VertexUV VertexType;

public:
    Cube(Vector3 size = Vector3(1, 1, 1));
    ~Cube();

    void Update();
    void Render();

private:
    void MakeMesh();

public :
    GameObject* gameObject;

private:
    Vector3 size;

    Mesh<VertexType>* mesh;
};