#include "Framework.h"

RockManager::RockManager(Model* model, string name, UINT count)
	: ModelInstancingManager(model, name, count)
{
	rocks.resize(count);
	for (UINT i = 0; i < count; i++)
	{
		Rocks* rock = new Rocks("Rock_0");
		rocks[i] = rock;
	}
	for (UINT i = 0; i < drawCount; i++)
	{
		rocks[i]->transform->Pos() = transforms[i]->Pos();
		rocks[i]->transform->Rot() = transforms[i]->Rot();
		rocks[i]->transform->Scale() = transforms[i]->Scale() * 1.2f;
		rocks[i]->Update();
	}
}

RockManager::~RockManager()
{
}

void RockManager::Update()
{
	ModelInstancingManager::Update();
	for (UINT i = 0; i < drawCount; i++)
	{
		rocks[i]->Update();
	}
}

void RockManager::Render()
{
	ModelInstancingManager::Render();
	for (Rocks* rock : rocks)
	{
		rock->Render();
	}
}

void RockManager::GUIRender()
{
}

void RockManager::PushCollider(Collider* collider)
{

	for (Rocks* rock : rocks)
	{
		rock->GetCollider()->PushCollision(collider);
	}
}

bool RockManager::IceAxeCollision(Collider* collider)
{
	for (Rocks* rock : rocks)
	{
		if (rock->GetCollider()->IsCollision(collider))
		{
			return true;
		}
	}
	return false;
}
