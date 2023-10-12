#include "Framework.h"

Rocks::Rocks(string name) : Model(name)
{
	collider = new SphereCollider();
	collider->transform->SetParent(transform);

}

Rocks::~Rocks()
{
}

void Rocks::Update()
{
	transform->UpdateWorld();
	//collider->transform->Pos().y = 0.5f;
	collider->transform->UpdateWorld();
}

void Rocks::Render()
{
	//Model::Render();
	collider->Render();
}

void Rocks::RenderInstance(int count)
{
	for (ModelMesh* mesh : meshes)
		mesh->RenderInstanced(count);
}

void Rocks::GUIRender()
{
	//Model::GUIRender();
	collider->transform->GUIRender();
}
