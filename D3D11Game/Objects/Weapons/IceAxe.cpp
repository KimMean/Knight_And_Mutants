#include "Framework.h"

IceAxe::IceAxe(string name) : Model(name)
{
	//transform->Load();
	collider = new BoxCollider();
	collider->transform->SetParent(transform);
	collider->transform->SetTag(name + "Collider");
	collider->transform->Load();

	materials[0]->LoadMaterial("TextData/Materials/Weapons/IceAxe.mat");
}

IceAxe::~IceAxe()
{
	delete collider;
}

void IceAxe::Update()
{
	transform->UpdateWorld();
	collider->transform->UpdateWorld();
}

void IceAxe::Render()
{
	Model::Render();
	collider->Render();
}

void IceAxe::GUIRender()
{
	Model::GUIRender();
	collider->transform->GUIRender();
}
