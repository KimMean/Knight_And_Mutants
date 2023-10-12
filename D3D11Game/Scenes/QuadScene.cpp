#include "Framework.h"
#include "QuadScene.h"

QuadScene::QuadScene()
{
	//quad = new Quad(Vector2(1.0f, 1.0f));
	//cube = new Cube();
	sphere = new Sphere();
	sphere->gameObject->material->SetDiffuseMap(L"Textures/Bricks.png");
	//sphere->gameObject->GetMaterial()->
	
	//boxCollider = new BoxCollider();
	//sphereCollider = new SphereCollider();
}

QuadScene::~QuadScene()
{
	//delete quad;
	//delete cube;
	delete sphere;
	//delete boxCollider;
	//delete sphereCollider;
}

void QuadScene::Update()
{
}

void QuadScene::Render()
{
	//quad->Render();
	//cube->Render();
	sphere->Render();
	//boxCollider->Render();
	//sphereCollider->Render();
}

void QuadScene::PreRender()
{
}

void QuadScene::PostRender()
{
}

void QuadScene::GUIRender()
{
}
