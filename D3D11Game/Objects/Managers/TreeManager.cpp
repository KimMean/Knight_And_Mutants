#include "Framework.h"

TreeManager::TreeManager(Model* model, string name, UINT count)
	: ModelInstancingManager(model, name, count)
{
	trees.resize(count);
	for (UINT i = 0; i < count; i++)
	{
		Tree* tree = new Tree("Lemon_Tree");
		trees[i] = tree;
	}
	ModelInstancingManager::Update();
	for (UINT i = 0; i < drawCount; i++)
	{
		trees[i]->transform->Pos() = transforms[i]->Pos();
		trees[i]->transform->Rot() = transforms[i]->Rot();
		trees[i]->Update();
	}
}

TreeManager::~TreeManager()
{
	for (UINT i = 0; i < drawCount; i++)
		delete trees[i];
}

void TreeManager::Update()
{
	ModelInstancingManager::Update();
	for (UINT i = 0; i < drawCount; i++)
	{
		trees[i]->transform->Pos() = transforms[i]->Pos();
		trees[i]->transform->Rot() = transforms[i]->Rot();
		trees[i]->Update();
	}
}

void TreeManager::Render()
{
	ModelInstancingManager::Render();
	for (Tree* tree : trees)
	{
		tree->Render();
	}
}

void TreeManager::GUIRender()
{
	ModelInstancingManager::GUIRender();
	//for (Tree* tree : trees)
	//{
	//	tree->GUIRender();
	//}
}

void TreeManager::PushCollider(Collider* collider)
{
	for (Tree* tree : trees)
	{
		tree->GetCollider()->PushCollision(collider);
	}
	//model->GetCollider()->PushCollision()
}

bool TreeManager::AxeCollision(Collider* collider)
{
	for (Tree* tree : trees)
	{
		if (tree->GetCollider()->IsCollision(collider)) 
		{
			return true;
		}
	}
	return false;
}
