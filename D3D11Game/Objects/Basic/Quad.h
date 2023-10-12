#pragma once

class Quad
{
private :
    typedef VertexUV VertexType;

public :
	Quad(Vector2 size);
	~Quad();

    void Render();

    void SetTexture(wstring file);
    Vector2 GetSize() { return size; }

    bool IsPointCollision(Vector3 pos);

private :
    void MakeMesh();

public :
    GameObject* gameObject;

private :
    Vector2 size;

    Mesh<VertexType>* mesh;

    Texture* texture = nullptr;
};