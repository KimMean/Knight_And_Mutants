#include "Framework.h"
#include "Build.h"

Build::Build(string name) : Model(name)
{
	SetShader(L"Shaders/Model/Model.hlsl");
	transform->Load();
	transform->UpdateWorld();

	collider = new BoxCollider();
	collider->transform->SetTag(name + "Collider");
	collider->transform->SetParent(transform);
	collider->transform->Load();
	collider->transform->UpdateWorld();

	stairs.resize(10);
	for (UINT i = 0; i < 10; i++)
	{
		stairs[i] = new BoxCollider(Vector3(70, 5, 20));
		stairs[i]->transform->SetTag("stairs" + to_string(i));
		stairs[i]->transform->SetParent(transform);
		stairs[i]->transform->Load();
		stairs[i]->transform->UpdateWorld();
	}


}

Build::~Build()
{
	delete collider;
}

void Build::Update()
{
	collider->transform->UpdateWorld();
	for (UINT i = 0; i < 10; i++)
	{
		stairs[i]->transform->UpdateWorld();
	}
}

void Build::Render()
{
	Model::Render();
	collider->Render();
	for (UINT i = 0; i < 10; i++)
	{
		stairs[i]->Render();
	}
}

void Build::GUIRender()
{
	if (ImGui::TreeNode("House"))
	{
		Model::GUIRender();

		if (ImGui::TreeNode("Colliders"))
		{
			collider->transform->GUIRender();

			for (UINT i = 0; i < 10; i++)
			{
				stairs[i]->transform->GUIRender();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void Build::PushCollider(Collider* collider)
{
	//if (this->collider->IsCollision(collider))
	//{
	//	float baseY = this->collider->transform->GlobalPos().y;
	//	float targetY = collider->transform->GlobalPos().y;
	//	if (baseY < targetY)
	//	{
	//		collider->transform->GetParent()->Pos().y = this->collider->transform->GlobalPos().y + 0.1f;
	//	}
	//}
	//this->collider->PushCollision(collider);
}
