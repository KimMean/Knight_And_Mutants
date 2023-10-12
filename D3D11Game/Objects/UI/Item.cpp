#include "Framework.h"
#include "Item.h"

Item::Item(Vector2 size) : Quad(size)
{
	gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
	count = 1;
}

Item::~Item()
{
}

void Item::Update()
{
	gameObject->transform->UpdateWorld();
}

void Item::Render()
{
	if(count > 0)
	{
		Quad::Render();
		if (count > 1)
		{
			Float2 xy = { gameObject->transform->Pos().x + 15.0f, gameObject->transform->Pos().y - 7.0f };
			Font::Get()->RenderText(to_string(count), xy, Float2(30, 10));
		}
	}
}

void Item::SetName(string name)
{
	this->name = name;
}

void Item::SetTexture(wstring textureFile)
{
	name = ToString(GetFileNameWithoutExtension(textureFile));
	
	gameObject->material->SetDiffuseMap(textureFile);
}

void Item::SetSlot(Slot* slot)
{
	if(this->slot)
		this->slot->isEmpty = true;
	this->slot = slot;
	if (slot)
	{
		slot->isEmpty = false;
		gameObject->transform->Pos() = slot->gameObject->transform->GlobalPos();
		gameObject->transform->UpdateWorld();
	}
}

//bool Item::IsPointCollision(Vector3 mousePos)
//{
//	Vector2 halfSize = GetSize() * 0.5f;
//	Vector3 pos = gameObject->transform->GlobalPos();
//
//	float x = mousePos.x - pos.x;
//	float y = mousePos.y - pos.y;
//
//	if (abs(x) < halfSize.x && abs(y) < halfSize.y)
//		return true;
//
//	return false;
//}

