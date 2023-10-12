#include "Framework.h"

SkyBox::SkyBox()
{
	skyBox = new Sphere();
	cubeMap = Texture::AddTexture(L"Textures/SkyBox/BlueSky.dds");
	skyBox->gameObject->material->SetShaderFile(L"Shaders/Landscape/SkyBox.hlsl");
}

SkyBox::~SkyBox()
{
	delete skyBox;
}

void SkyBox::SetSkyBoxTexture(wstring textureFile)
{
	cubeMap = Texture::AddTexture(textureFile);
}

void SkyBox::Render()
{
	cubeMap->SetPixelShaderSlot(10);

	Environment::Get()->SetFrontCounterClockwise(true);	// 안쪽면 그리고
	Environment::Get()->SetDepthEnable(false);	// 뎁스 적용 꺼놓기

	Environment::Get()->SetRasterizerState();
	Environment::Get()->SetDepthStencilState();
	
	skyBox->Render();

	Environment::Get()->SetDepthEnable(true);
	Environment::Get()->SetFrontCounterClockwise(false);

	Environment::Get()->SetRasterizerState();
	Environment::Get()->SetDepthStencilState();
}
