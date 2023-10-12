#pragma once

class Material
{
private :
    enum MapType
    {
        DIFFUSE, SPECULAR, NORMAL
    };

public :
    struct MaterialData
    {
        Float4 diffuse = { 1, 1, 1, 1 };
        Float4 specular = { 1, 1, 1, 1 };
        Float4 ambient = { 1, 1, 1, 1 };
        Float4 emissive = { 0, 0, 0, 1 };

        float shininess = 24.0f;
        int hasNormalMap = 0;
        float padding[2];
    };

public :
    Material();
    ~Material();

    void GUIRender();

    void SetMaterial();

    void SetShaderFile(wstring shaderFile = L"Shaders/Light/Light.hlsl");
    void SetDiffuseMap(Texture* texture) { diffuseMap = texture; }
    void SetDiffuseMap(wstring textureFile = L"");
    void SetSpecularMap(wstring textureFile = L"");
    void SetNormalMap(wstring textureFile = L"");

    Texture* GetDiffuseMap() { return diffuseMap; }
    Texture* GetSpecularMap() { return specularMap; }
    Texture* GetNormalMap() { return normalMap; }

    MaterialData& GetMaterialData() { return materialData; }
    string& GetName() { return name; }

    void SaveMaterial(string filePath);
    void LoadMaterial(string filePath);

private:
    void SelectMap(string name, MapType type);
    void UnselectMap(MapType type);

    void SaveDialog();
    void LoadDialog();
private :
    string name;
    string file;
    string projectPath;

    ConstBuffer* constBuffer;
    MaterialData materialData;

    VertexShader* vertexShader = nullptr;
    PixelShader* pixelShader = nullptr;

    Texture* diffuseMap = nullptr;
    Texture* specularMap = nullptr;
    Texture* normalMap = nullptr;

};