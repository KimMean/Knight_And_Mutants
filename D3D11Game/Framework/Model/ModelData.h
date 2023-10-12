#pragma once


typedef VertexUVNormalTangentBlend ModelVertex;

// 노드 정보
struct NodeData
{
    int index;
    string name;
    int parent;
    Matrix transform;
};

// 메쉬 정보
struct MeshData
{
	string name;
	UINT materialIndex;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};

// 뼈대 정보
struct BoneData
{
    int index;
    string name;
    Matrix offset;
};

// 접히는 부분 가중치 정보
struct VertexWeights
{
    UINT indices[4] = {};//Bone Index;
    float weights[4] = {};//보간값에 대한 가중치

    void AddWeight(const UINT& index, const float& weight)
    {
        for (int i = 0; i < 4; i++)
        {
            if (weights[i] == 0.0f)
            {
                indices[i] = index;
                weights[i] = weight;
                return;
            }
        }
    }

    void Normalize()
    {
        float sum = 0.0f;

        for (int i = 0; i < 4; i++)
            sum += weights[i];

        for (int i = 0; i < 4; i++)
            weights[i] /= sum;
    }
};


struct KeyVector
{
    float time;     // 시간
    Float3 value;   // 포지션
};

struct KeyQuat
{
    float time;     // 시간
    Float4 value;   // 로테이션 값
};

struct KeyData
{
    vector<KeyVector> scales;   // 스케일
    vector<KeyQuat> rotations;  // 로테이션
    vector<KeyVector> positions;    // 포지션
};

struct KeyTransform
{
    Float3 scale;   // 스케일
    Float4 rot;     // 로테이션
    Float3 pos;     // 포지션
};

struct KeyFrame
{
    string boneName;    // 뼈대
    vector<KeyTransform> transforms;    // 트랜스폼
};

struct ClipNode
{
    aiString name;  // 이름
    vector<KeyTransform> transforms;    // 트랜스폼
};

struct Clip
{
    string name;    // 이름

    UINT frameCount;    // 프레임
    float tickPerSecond;    // 초당 틱

    vector<KeyFrame*> keyFrame; // 키프레임
};

struct ClipTransform
{
    Matrix transform[MAX_FRAME][MAX_BONE];
};