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

    // 터레인 조정 범위 브러쉬
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

    // 지형에 쏘는 Ray Data
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

    // CS에 정점 3개를 입력 하기위한 구조체
    struct InputDesc
    {
        Float3 v0, v1, v2;
    };

    // CS에서 처리된 Ray충돌 인덱스와 거리
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
    BrushBuffer* brushBuffer;       // 브러쉬
    RayBuffer* rayBuffer;           // 레이

    vector<InputDesc> inputs;       // 정점 입력
    vector<OutputDesc> outputs;     // 범위 출력

    ComputeShader* computeShader;   // 컴퓨트 쉐이더
    ComputeBuffer* computeBuffer;   // 컴퓨트 버퍼

private :
    string projectPath;
};