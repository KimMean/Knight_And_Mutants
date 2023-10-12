#pragma once


typedef VertexUVNormalTangentBlend ModelVertex;

// ��� ����
struct NodeData
{
    int index;
    string name;
    int parent;
    Matrix transform;
};

// �޽� ����
struct MeshData
{
	string name;
	UINT materialIndex;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};

// ���� ����
struct BoneData
{
    int index;
    string name;
    Matrix offset;
};

// ������ �κ� ����ġ ����
struct VertexWeights
{
    UINT indices[4] = {};//Bone Index;
    float weights[4] = {};//�������� ���� ����ġ

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
    float time;     // �ð�
    Float3 value;   // ������
};

struct KeyQuat
{
    float time;     // �ð�
    Float4 value;   // �����̼� ��
};

struct KeyData
{
    vector<KeyVector> scales;   // ������
    vector<KeyQuat> rotations;  // �����̼�
    vector<KeyVector> positions;    // ������
};

struct KeyTransform
{
    Float3 scale;   // ������
    Float4 rot;     // �����̼�
    Float3 pos;     // ������
};

struct KeyFrame
{
    string boneName;    // ����
    vector<KeyTransform> transforms;    // Ʈ������
};

struct ClipNode
{
    aiString name;  // �̸�
    vector<KeyTransform> transforms;    // Ʈ������
};

struct Clip
{
    string name;    // �̸�

    UINT frameCount;    // ������
    float tickPerSecond;    // �ʴ� ƽ

    vector<KeyFrame*> keyFrame; // Ű������
};

struct ClipTransform
{
    Matrix transform[MAX_FRAME][MAX_BONE];
};