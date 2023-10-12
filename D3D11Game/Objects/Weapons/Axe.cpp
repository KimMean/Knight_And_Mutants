#include "Framework.h"

Axe::Axe(string name) : Model(name)
{
	//transform->Load();
	collider = new BoxCollider();
	collider->transform->SetParent(transform);
	collider->transform->SetTag(name + "Collider");
	collider->transform->Load();

	materials[0]->LoadMaterial("TextData/Materials/Weapons/Axe.mat");
}

Axe::~Axe()
{
	delete collider;
}

void Axe::Update()
{
	transform->UpdateWorld();
	collider->transform->UpdateWorld();
}

void Axe::Render()
{
	Model::Render();
	collider->Render();
}

void Axe::GUIRender()
{
	Model::GUIRender();
	collider->transform->GUIRender();
}
