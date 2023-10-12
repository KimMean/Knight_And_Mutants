#pragma once

class Model
{
public:
    Model(string name);
    ~Model();

    void Render();
    virtual void RenderInstance(int count) {}
    void GUIRender();

    void SetShader(wstring file);
    Material* AddMaterial();
    Material* AddMaterial(string materialName);
    vector<ModelMesh*> GetMeshs() { return meshes; }
    ModelMesh* GetMesh(UINT index) { return meshes[index]; }
    UINT GetMeshNum() { return meshes.size(); }
    
    string GetName() { return name; }
    vector<Material*> GetMaterials() { return materials; }
    
private:
    void ReadMaterial();
    void ReadMesh();

public :
    Transform* transform;

protected:
    string name;

    vector<Material*> materials;
    vector<ModelMesh*> meshes;
    vector<NodeData> nodes;
    vector<BoneData> bones;

    map<string, UINT> boneMap;

    //MatrixBuffer* worldBuffer;    // Transfrom º¸À¯
};