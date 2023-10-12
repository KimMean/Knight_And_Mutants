#include "Framework.h"

ModelInstancingManager::ModelInstancingManager()
{
	//vertexShader = Shader::AddVertexShader(L"Shaders/Model/ModelInstancing.hlsl");
	//pixelShader = Shader::AddPixelShader(L"Shaders/Model/ModelInstancing.hlsl");

	// 테스트용
	//model = new Tree("Lemon_Tree");
	//model->SetShader(L"Shaders/Model/ModelInstancing.hlsl");
	//
	//for (float z = 0; z < 5; z++)
	//{
	//	for (float x = 0; x < 5; x++)
	//	{
	//		UINT index = 5 * z + x;
	//		Transform* transform = new Transform();
	//		transform->SetTag("Tree" + to_string(index));
	//		//transform->SetTag(to_string(10 * z + x));
	//		//transform->Pos() = { x * 10, 0.0f, z * 10 };
	//		//transform->Scale() = { 2.0f, 2.0f, 2.0f };
	//		transform->Load();
	//		transform->UpdateWorld();
	//		transforms.push_back(transform);
	//		// 인스턴싱 데이터에 트랜스폼 행렬 입력
	//		instanceDatas[drawCount].world = XMMatrixTranspose(transform->GetWorld());
	//		//instanceDatas[drawCount].index = drawCount;
	//		drawCount++;
	//	}
	//}
	//vertexBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
	//vertexBuffer->IASet(1);
}

ModelInstancingManager::ModelInstancingManager(Model* model, string name, UINT count) : name(name)
{
	this->model = model;
	model->SetShader(L"Shaders/Model/ModelInstancing.hlsl");
	for (int i = 0; i < count; i++)
	{
		Transform* transform = new Transform();
		transform->SetTag(name + to_string(i));

		transform->Load(name);
		//if (name._Equal("Rock_"))
		//{
		//	int x = i % 5;
		//	int z = i / 5;
		//	float rndValue = Random(5.0f, 16.0f);
		//	transform->Pos() = { x * rndValue + rndValue, 10.0f, z * rndValue + rndValue };
		//	transform->Rot() = { Random(-3.0f, 3.0f), Random(-3.0f, 3.0f) , Random(-3.0f, 3.0f) };
		//	rndValue = Random(0.3f, 1.0f);
		//	transform->Scale() = { rndValue, rndValue, rndValue };
		//}
		transform->UpdateWorld();
		transforms.push_back(transform);

		instanceDatas[drawCount].world = XMMatrixTranspose(transform->GetWorld());

		drawCount++;
	}
	vertexBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
	vertexBuffer->IASet(1);
}

ModelInstancingManager::~ModelInstancingManager()
{
	delete vertexBuffer;

	for (Transform* transform : transforms)
		delete transform;

	delete model;
}

void ModelInstancingManager::Update()
{

	drawCount = 0;
	for (Transform* transform : transforms)
	{
		transform->UpdateWorld();
		instanceDatas[drawCount].world = XMMatrixTranspose(transform->GetWorld());
		//instanceDatas[drawCount].index = drawCount;
		drawCount++;
	}
	vertexBuffer->Update(instanceDatas, drawCount);
	//vertexBuffer->IASet(1);
}

void ModelInstancingManager::Render()
{
	vertexBuffer->IASet(1);
	model->RenderInstance(drawCount);
}

void ModelInstancingManager::GUIRender()
{
	string str = name + "Models";
	if (ImGui::TreeNode(str.c_str()))
	{
		for (Transform* transform : transforms)
			transform->GUIRender();
		ImGui::TreePop();
	}
}
