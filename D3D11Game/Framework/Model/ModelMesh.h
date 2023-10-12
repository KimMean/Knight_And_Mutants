#pragma once

class ModelMesh
{
public:
    ModelMesh(string name);
    ~ModelMesh();

    void Render();

    void RenderInstanced(UINT instanceCount);

    void CreateMesh(void* vertexData, UINT vertexCount, void* indexData, UINT indexCount);

    Material* GetMaterial() { return material; }
    void SetMaterial(Material* material) { this->material = material; }
private:
    string name;

    Material* material;
    Mesh<ModelVertex>* mesh;
};