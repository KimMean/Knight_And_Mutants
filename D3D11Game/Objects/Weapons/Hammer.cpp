#include "Framework.h"

Hammer::Hammer(string name) : Model(name)
{
	transform->Load();
	collider = new BoxCollider();
	collider->transform->SetParent(transform);
	collider->transform->SetTag(name + "Collider");
	collider->transform->Load();

	materials[0]->LoadMaterial("TextData/Materials/Weapons/Hammer.mat");
}

Hammer::~Hammer()
{
	delete collider;
}

void Hammer::Update()
{
	transform->UpdateWorld();
	collider->transform->UpdateWorld();
}

void Hammer::Render()
{
	Model::Render();
	collider->Render();
}

void Hammer::GUIRender()
{
	Model::GUIRender();
	collider->transform->GUIRender();
}
