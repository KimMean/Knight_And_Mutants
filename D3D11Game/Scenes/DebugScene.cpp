#include "Framework.h"
#include "DebugScene.h"

DebugScene::DebugScene()
{
}

DebugScene::~DebugScene()
{
}

void DebugScene::Update()
{
	if (KEY_DOWN(VK_F1))
	{
		isWireFrameMode = !isWireFrameMode;
		// WireFrameMode
		if (isWireFrameMode)
			Environment::Get()->SetFillMode(D3D11_FILL_WIREFRAME);
		else 
			Environment::Get()->SetFillMode();
	}
}

void DebugScene::PreRender()
{
}

void DebugScene::Render()
{
}

void DebugScene::PostRender()
{
}

void DebugScene::GUIRender()
{
}
