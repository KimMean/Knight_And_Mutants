#include "Framework.h"

Slot::Slot(Vector2 size) : Quad(size)
{
	gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
	//gameObject->material->SetShaderFile(L"Shaders/UI/Slot.hlsl");
	gameObject->material->SetDiffuseMap(L"Textures/Icons/Slot.png");
}

Slot::~Slot()
{
}
