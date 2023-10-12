#include "Framework.h"

Sword::Sword(string name) : Model(name)
{
	collider = new BoxCollider();
	collider->transform->SetParent(transform);
	collider->transform->SetTag(name + "Collider");
	collider->transform->Load();

	materials[0]->LoadMaterial("TextData/Materials/Weapons/Sword.mat");
}

Sword::~Sword()
{
	delete collider;
}

void Sword::Update()
{
	transform->UpdateWorld();
	collider->transform->UpdateWorld();
}

void Sword::Render()
{
	Model::Render();
	collider->Render();
}

void Sword::GUIRender()
{
	Model::GUIRender();
	collider->transform->GUIRender();
}
