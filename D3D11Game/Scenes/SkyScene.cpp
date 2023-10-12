#include "Framework.h"
#include "SkyScene.h"

SkyScene::SkyScene()
{
	skyBox = new SkyBox();
	water = new Water();
	//sky = new Sphere();
	//cubeMap = Texture::AddTexture(L"Textures/SkyBox/BlueSky.dds");
	//sky->gameObject->GetMaterial()->SetShaderFile(L"Shaders/Landscape/SkyBox.hlsl");

	//quad = new Quad(Vector2(500, 500));
	//quad->gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
	//quad->gameObject->transform->Pos() = { 0, 0, 0 };
	//Texture* texture = Texture::AddTexture(L"Reflection", renderTarget->GetSRV());
	////quad->gameObject->material->SetDiffuseMap(texture);
	//quad->SetTexture(L"Textures/Landscape/Dirt.png");
	//quad->gameObject->transform->UpdateWorld();
}

SkyScene::~SkyScene()
{
}

void SkyScene::Update()
{
	water->Update();
}

void SkyScene::PreRender()
{
}

void SkyScene::Render()
{
	skyBox->Render();
	water->Render();
	//water->SetReflection();
	//skyBox->Render();
	//water->Render();

	//Environment::Get()->SetState();
	//quad->Render();
	//cubeMap->SetPixelShaderSlot(10);
	//
	//Environment::Get()->SetFrontCounterClockwise(true);	// 안쪽면 그리고
	//Environment::Get()->SetDepthEnable(false);	// 뎁스 적용 꺼놓기
	//
	//Environment::Get()->SetRasterizerState();
	//Environment::Get()->SetDepthStencillState();
	//
	//sky->Render();
	//
	//Environment::Get()->SetDepthEnable(true);
	//Environment::Get()->SetFrontCounterClockwise(false);
	//
	//Environment::Get()->SetRasterizerState();
	//Environment::Get()->SetDepthStencillState();
}

void SkyScene::PostRender()
{
	water->PostRender();
	//quad->Render();
}

void SkyScene::GUIRender()
{
	water->GUIRender();
}
