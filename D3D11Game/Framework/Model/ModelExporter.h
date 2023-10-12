#pragma once

class ModelExporter
{
public :
	ModelExporter();
	~ModelExporter();

	void ReadFile(string modelName, string filePath);
	void ExportMesh();
	void ExportMaterial();
	void ExportClip(string clipName);

private :
	//Mesh
	void ReadNode(aiNode* node, int index, int parent);
	void ReadMesh(aiNode* node);
	void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights);
	void WriteMesh();

	//Material
	void ReadMaterial();
	void WriteMaterial();
	string CreateTexture(string file);
	
    //Clip
    Clip* ReadClip(aiAnimation* animation);
    void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes);
    void WriterClip(Clip* clip, string clipName, UINT index);
    void SetClipNode(const KeyData& keyData, const UINT& frameCount, ClipNode& clipNode);
    Float3 CalcInterpolationVector(const vector<KeyVector>& keyData, UINT& count, int curFrame);
    Float4 CalcInterpolationQuat(const vector<KeyQuat>& keyData, UINT& count, int curFrame);

private :
	string name;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<NodeData*> nodes;
	vector<BoneData*> bones;
	vector<MeshData*> meshes;
	vector<Material*> materials;

	map<string, UINT> boneMap;
	UINT boneCount = 0;
};