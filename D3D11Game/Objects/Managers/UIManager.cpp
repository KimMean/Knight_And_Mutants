#include "Framework.h"
#include "UIManager.h"

UIManager::UIManager()
{
	inventory = new Inventory();

	interaction = new Interaction(Vector2(150, 50));

	hpBar = new ProgressBar(Vector2(260, 40));
	hpBar->gameObject->transform->Pos() = { 150, 50 };
	hpBar->SetTexture(L"Textures/Icons/HP_BG.png");
	hpBar->SetFrontImage(Vector2(215, 30), L"Textures/Icons/HP.png");
	hpBar->frontImage->gameObject->transform->Pos() = { 17.0f, -0.5f };
	//hpBar->SetAmount(0.5f);

	textBox = new TextBox();
}

UIManager::~UIManager()
{
	delete inventory;
	delete interaction;
	delete hpBar;
	delete textBox;
}

void UIManager::Update()
{
	if(inventoryIsShow)
		inventory->Update();
}

void UIManager::PostRender()
{
	if(isInteraction && !textBoxIsShow)
		interaction->Render();
	inventory->PostRender();
	hpBar->SetRender();
	hpBar->Render();
	if(textBoxIsShow)
		textBox->Render();
}
