#include "Framework.h"
#include "ModelMesh.h"

ModelMesh::ModelMesh(string name)
{
}

ModelMesh::~ModelMesh()
{
    delete mesh;
}

void ModelMesh::Render()
{
    // 메테리얼 설정 후 그리기
    material->SetMaterial();
    mesh->Draw();
}

void ModelMesh::RenderInstanced(UINT instanceCount)
{
    material->SetMaterial();
    mesh->DrawInstanced(instanceCount);
}

void ModelMesh::CreateMesh(void* vertexData, UINT vertexCount, void* indexData, UINT indexCount)
{
    mesh = new Mesh<ModelVertex>();
    // 버텍스
    mesh->GetVertices().resize(vertexCount);
    memcpy(mesh->GetVertices().data(), vertexData, sizeof(ModelVertex) * vertexCount);
    // 인덱스
    mesh->GetIndices().resize(indexCount);
    memcpy(mesh->GetIndices().data(), indexData, sizeof(UINT) * indexCount);

    mesh->CreateMesh();
}
