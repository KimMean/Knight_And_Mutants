#include "Framework.h"

ProgressBar::ProgressBar(Vector2 size) : Quad(size)
{
	// 프로그래스바로 변경
	gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
}

ProgressBar::~ProgressBar()
{
	delete frontImage;
}

void ProgressBar::Update()
{
	gameObject->transform->UpdateWorld();
	if (frontImage)
	{
		//frontImage->gameObject->transform->Pos().z += 0.0001f;
		frontImage->gameObject->transform->UpdateWorld();
	}
}

void ProgressBar::Render()
{
	Quad::Render();
	if (frontImage)
	{
		//valueBuffer->SetPixelShaderBuffer(10);
		frontImage->Render();
	}
}

void ProgressBar::SetRender()
{
	valueBuffer->SetPixelShaderBuffer(10);
}

void ProgressBar::SetAmount(float value)
{
	fillAmount = value;
	valueBuffer->Get()[0] = fillAmount;
}

void ProgressBar::SetBackImage(Vector2 size, wstring textureFile)
{
	SetTexture(textureFile);
	//gameObject->material->SetDiffuseMap(textureFile);
}

void ProgressBar::SetFrontImage(Vector2 size, wstring textureFile)
{
	frontImage = new Quad(size);
	frontImage->gameObject->material->SetShaderFile(L"Shaders/UI/Progress.hlsl");
	frontImage->gameObject->transform->SetParent(gameObject->transform);
	frontImage->SetTexture(textureFile);
	frontImage->gameObject->transform->UpdateWorld();
	valueBuffer = new FloatValueBuffer();
}
