#include "Framework.h"

Cube::Cube(Vector3 size)
	: size(size)
{
    gameObject = new GameObject();

	mesh = new Mesh<VertexType>();
	MakeMesh();
	mesh->CreateMesh();
}

Cube::~Cube()
{
	delete mesh;
}

void Cube::Update()
{
    gameObject->transform->UpdateWorld();
}

void Cube::Render()
{
    gameObject->SetRender();
	mesh->Draw();
}

void Cube::MakeMesh()
{
	Vector3 halfSize = size * 0.5f;

	vector<VertexType>& vertices = mesh->GetVertices();
	//Front
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 1, 0);
    //Up
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 1, 0);
    //Left
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 0);

    //Right
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 1, 1);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 1, 0);

    //Back
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 0, 1);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 0, 0);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 0);

    //Down
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 0, 0);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 1, 1);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 0);

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        //Front
        0, 1, 2, 2, 1, 3,
        //Up
        4, 5, 6, 6, 5, 7,
        //Right
        8, 9, 10, 10, 9, 11,
        //Left
        12, 13, 14, 14, 13, 15,
        //Back
        16, 17, 18, 18, 17, 19,
        //Down
        20, 21, 22, 22, 21, 23
    };
}
