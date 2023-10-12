#pragma once

class TerrainEditor
{
private:
    enum ADJUST_TYPE
    {
        FLAT = 0,
        DIG = 1,
        BUILD = 2
    };

    typedef VertexUVNormalTangentAlpha VertexType;

    const float MIN_HEIGHT = -5.0f;
    const float MAX_HEIGHT = +15.0f;

    // �ͷ��� ���� ���� �귯��
    class BrushBuffer : public ConstBuffer
    {
    private:
        struct Data
        {
            int type = 0;
            Float3 pickingPos;

            float range = 10.0f;
            Float3 color = { 0, 1, 0 };
        };

    public:
        BrushBuffer() : ConstBuffer(&data, sizeof(Data))
        {
        }

        Data& Get() { return data; }

    private:
        Data data;
    };

    // ������ ��� Ray Data
    class RayBuffer : public ConstBuffer
    {
    private:
        struct Data
        {
            Float3 pos;
            UINT triangleSize;

            Float3 dir;
            float padding;
        };

    public:
        RayBuffer() : ConstBuffer(&data, sizeof(Data))
        {
        }

        Data& Get() { return data; }

    private:
        Data data;
    };

    // CS�� ���� 3���� �Է� �ϱ����� ����ü
    struct InputDesc
    {
        Float3 v0, v1, v2;
    };

    // CS���� ó���� Ray�浹 �ε����� �Ÿ�
    struct OutputDesc
    {
        int picked;
        float distance;
    };

public :
    TerrainEditor();
    ~TerrainEditor();

    void Update();
    void Render();
    void GUIRender();

public :
    bool ComputePicking();

    void CreateTerrain(Vector2 size);

private :
    void Resize();
    void AdjustHeight();
    void UpdateHeight();
    void UpdateBlendTexture();
    void AdjustBlendTexture();

private:
    void MakeMesh();
    void MakeNormalVector();
    void MakeTangentVector();
    void MakeComputeData();

    void SaveHeightMap();
    void LoadHeightMap();

    void SaveAlphaMap();
    void LoadAlphaMap();

public :
    GameObject* gameObject;
private :
    Mesh<VertexType>* mesh;

    Vector2 terrainSize;
    UINT triangleSize;
    Vector3 pickingPos;

    // adjust
    ADJUST_TYPE adjustType = BUILD;
    float adjustValue = 10.0f;
    float minHeight;
    float maxHeight;

    bool isLoadComplite = true;
    UINT selectMap;
    Texture* heightMap;
    Texture* sandTexture;
    Texture* grassTexture;
    Texture* soilTexture;


// Editing
private :
    BrushBuffer* brushBuffer;       // �귯��
    RayBuffer* rayBuffer;           // ����

    vector<InputDesc> inputs;       // ���� �Է�
    vector<OutputDesc> outputs;     // ���� ���

    ComputeShader* computeShader;   // ��ǻƮ ���̴�
    ComputeBuffer* computeBuffer;   // ��ǻƮ ����

private :
    string projectPath;
};