#include "Framework.h"

Tree::Tree(string name) : Model(name)
{
	SetShader(L"Shaders/Model/Model.hlsl");
	//SetShader(L"Shaders/Light/Shadow.hlsl");
	//model->SetShader(L"Shaders/Model/Model.hlsl");

	transform->UpdateWorld();
	collider = new CapsuleCollider(0.3f, 1.0f);
	collider->transform->SetParent(transform);
	collider->transform->Pos().y = 0.5f;
	collider->transform->UpdateWorld();
}

Tree::~Tree()
{
	delete collider;
}

void Tree::Update()
{
	transform->UpdateWorld();
	collider->transform->Pos().y = 0.5f;
	collider->transform->UpdateWorld();
}

void Tree::Render()
{
	//Environment::Get()->SetAlphaBlend(true);
	//Environment::Get()->SetAlphaToCoverage(true);
	//Environment::Get()->SetBlendState();
	//Model::Render();
	//Environment::Get()->SetAlphaToCoverage(false);
	//Environment::Get()->SetBlendState();
	collider->Render();
}

void Tree::RenderInstance(int count)
{
	meshes[0]->RenderInstanced(count);

	Environment::Get()->SetAlphaToCoverage(true);
	Environment::Get()->SetBlendState();
	meshes[1]->RenderInstanced(count);
	Environment::Get()->SetAlphaToCoverage(false);
	Environment::Get()->SetBlendState();
}

void Tree::GUIRender()
{
	//Model::GUIRender();
	collider->transform->GUIRender();
}
