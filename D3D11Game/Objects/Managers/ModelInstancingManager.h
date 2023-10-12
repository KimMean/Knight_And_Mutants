#pragma once

class ModelInstancingManager
{
private :
	struct InstanceData
	{
		Matrix world = XMMatrixIdentity();
	};

public :
	ModelInstancingManager();
	ModelInstancingManager(Model* model, string name, UINT count);
	~ModelInstancingManager();

	void Update();
	void Render();
	void GUIRender();


protected :
	string name;
	//Tree* models;
	Model* model;

	vector<Transform*> transforms;
	InstanceData instanceDatas[MAX_INSTANCE];

	VertexBuffer* vertexBuffer;
	UINT drawCount = 0;

};