#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	model = new Model("IceAxe");
	model->SetShader(L"Shaders/Model/Model.hlsl");
	//model->transform->SetTag("Axe");


	//modelAnimator = new ModelAnimator("Mutant");
	////modelAnimator->PlayClip(0, 1.0f, 0.1f);
	//modelAnimator->ReadClip("Idle");
	//modelAnimator->ReadClip("Walk");
	//modelAnimator->ReadClip("Attack");
	//modelAnimator->ReadClip("Roar");
	//modelAnimator->ReadClip("Death");


	
}

ModelRenderScene::~ModelRenderScene()
{
	delete model;
	delete modelAnimator;
}

void ModelRenderScene::Update()
{
	model->transform->UpdateWorld();
	if (KEY_DOWN('1'))
		modelAnimator->PlayClip(0);
	if (KEY_DOWN('2'))
		modelAnimator->PlayClip(1);
	if (KEY_DOWN('3'))
		modelAnimator->PlayClip(2);
	if (KEY_DOWN('4'))
		modelAnimator->PlayClip(3);
	if (KEY_DOWN('5'))
		modelAnimator->PlayClip(4);
	if (KEY_DOWN('6'))
		modelAnimator->PlayClip(5);
	if (KEY_DOWN('7'))
		modelAnimator->PlayClip(6);
	if (KEY_DOWN('8'))
		modelAnimator->PlayClip(7);
	if (KEY_DOWN('9'))
		modelAnimator->PlayClip(8);
	//modelAnimator->Update();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
	model->Render();
	//modelAnimator->Render();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
	model->GUIRender();
	//modelAnimator->GUIRender();
}
