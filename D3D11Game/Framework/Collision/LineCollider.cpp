#include "Framework.h"

LineCollider::LineCollider(Vector3 pos, Vector3 dir, float distance) : position(pos), direction(dir), distance(distance)
{
    world = new MatrixBuffer();
    vertexShader = Shader::AddVertexShader(L"Shaders/Basic/Collider.hlsl");
    pixelShader = Shader::AddPixelShader(L"Shaders/Basic/Collider.hlsl");

    mesh = new Mesh<VertexColor>();
	MakeMesh();
	mesh->CreateMesh();
}

LineCollider::~LineCollider()
{
}

void LineCollider::Render()
{
    world->SetVertexShaderBuffer(0);

    vertexShader->SetShader();
    pixelShader->SetShader();

    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void LineCollider::SetLine(Vector3 pos, Vector3 dir)
{
    position = pos;
    direction = dir;
    mesh->GetVertices()[0].pos = pos;
    mesh->GetVertices()[1].pos = pos + direction * distance;
    mesh->UpdateVertex();
}

void LineCollider::MakeMesh()
{
    Vector3 dirPos = direction * distance;

    vector<VertexColor>& vertices = mesh->GetVertices();

    vertices.emplace_back(0.0f, 0.0f, 0.0f, COLOR_GREEN);
    vertices.emplace_back(dirPos.x, dirPos.y, dirPos.z, COLOR_GREEN);
    
    vector<UINT>& indices = mesh->GetIndices();

    indices = {
        0, 1
    };
}
