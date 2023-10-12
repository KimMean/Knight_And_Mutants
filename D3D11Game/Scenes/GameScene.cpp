#include "Framework.h"
#include "GameScene.h"

GameScene::GameScene()
{
	Create();
}

GameScene::~GameScene()
{
	Delete();
}

void GameScene::Update()
{
	UIManager::Get()->Update();

	aStar->Update();
	player->Update();
	//monster->Update();
	npcManager->Update();
	UIManager::Get()->IsInteraction() = npcManager->IsCollisionNPC(player->GetCollider());
	water->Update();
	//house->Update();
	//treeManager->Update();
	//rockManager->Update();
	monsterManager->Update();
	monsterManager->TrackingPlayer(player->GetCollider());
	
	CollisionCheck();

	if (KEY_DOWN('G'))
		monsterManager->Spawn();

	if (KEY_DOWN(VK_ESCAPE))
		PostQuitMessage(0);

}

void GameScene::PreRender()
{
	// ½¦µµ¿ì ·»´õ Å¸°Ù ºä ¼³Á¤
	// µª½º¸Ê ·»´õ
	//shadow->SetRenderTarget();
	//
	////monster->SetShader(L"Shaders/Light/AnimationDepthMap.hlsl");
	////monster->Render();
	////
	//terrain->gameObject->material->SetShaderFile(L"Shaders/Light/DepthMap.hlsl");
	//terrain->Render();
	//
	//player->SetShader(L"Shaders/Light/AnimationDepthMap.hlsl");
	//player->Render();
	//
	//house->SetShader(L"Shaders/Light/DepthMap.hlsl");
	//house->Render();
	// ³ª¹«, µ¹, Áý µª½º¸Ê ¼³Á¤ ÈÄ ·»´õ
	//monsterManager->SetShader(L"Light/DepthMap.hlsl");
	//monsterManager->PreRender();
}

void GameScene::Render()
{
	skyBox->Render();
	terrain->Render();
	water->Render();

	house->Render();
	treeManager->Render();
	rockManager->Render();

	player->Render();
	npcManager->Render();
	monsterManager->Render();

	aStar->Render();
}

void GameScene::PostRender()
{
	//shadow->PostRender();
	UIManager::Get()->PostRender();
	monsterManager->PostRender();
	
}

void GameScene::GUIRender()
{
	//tree->GUIRender();
	house->GUIRender();
	treeManager->GUIRender();
	//treeInstancing->GUIRender();
	rockManager->GUIRender();
	player->GUIRender();
	//monster->GUIRender();
	npcManager->GUIRender();
	monsterManager->GUIRender();

	//shadow->GUIRender();
}

void GameScene::CollisionCheck()
{
	player->FloorCollision(house->GetCollider());
	for(UINT i = 0; i < 10; i++)
		player->FloorCollision(house->GetCollider(i));
	//house->PushCollider(player->GetCollider());
	treeManager->PushCollider(player->GetCollider());
	rockManager->PushCollider(player->GetCollider());
}

void GameScene::Create()
{
	// SkyBox
	skyBox = new SkyBox();
	// Terrain
	terrain = new Terrain();
	aStar = new AStar(64, 64);
	aStar->SetNode(terrain);
	aStar->LoadNodeState();
	// Water
	water = new Water();
	// °Ç¹°
	house = new Build("pier_house");
	
	// Tree
	treeManager = new TreeManager(new Tree("Lemon_Tree"), "Tree", 25);
	rockManager = new RockManager(new Rocks("Rock_0"), "Rock_", 25);
	//treeInstancing = new ModelInstancingManager(new Tree("Lemon_Tree"), "Tree", 25);
	//rockInstancing = new ModelInstancingManager(new Rocks("Rock_0"), "Rock_", 25);

	player = new Knight();
	npcManager = new NPCManager();
	monsterManager = new MonsterManager();

	player->SetTerrain(terrain);
	player->SetTreeManager(treeManager);
	player->SetRockManager(rockManager);
	player->SetMonsterManager(monsterManager);

	monsterManager->SetTerrain(terrain);
	monsterManager->SetAStar(aStar);
	monsterManager->SetTarget(player);
	
	//shadow = new Shadow(1000, 1000);
	//terrain->SetDepthTexture(shadow->GetTexture());

	UIManager::Get();

	CAM->SetTarget(player->transform);
	CAM->TargetOptionLoad("Option");
	CAM->LookAtTarget();

	CreateSound();

}

void GameScene::Delete()
{
	delete skyBox;
	delete water;
	delete terrain;
	delete aStar;
	delete house;
	delete treeManager;
	delete rockManager;
	delete player;
	//delete monster;
	delete npcManager;
	delete monsterManager;
	//delete shadow;
	UIManager::Delete();
}

void GameScene::CreateSound()
{
	// È¯°æ
	Audio::Get()->Add("Environment", "Sounds/Environment.mp3", true, true);
	Audio::Get()->Add("Hammer", "Sounds/Hammer.wav");
	Audio::Get()->Add("Wood", "Sounds/AxeWood.wav");
	Audio::Get()->Add("Stone", "Sounds/Stone.mp3");
	// Ä³¸¯ÅÍ
	Audio::Get()->Add("Hit", "Sounds/Hit.wav");
	Audio::Get()->Add("Jump", "Sounds/Jump.wav");
	Audio::Get()->Add("Sword", "Sounds/Sword.mp3");

	// Monster
	Audio::Get()->Add("Roar", "Sounds/Roar.mp3");
	Audio::Get()->Add("MHit", "Sounds/MonsterHit.mp3");
	Audio::Get()->Add("MAttack", "Sounds/MonsterAttack.mp3");
	Audio::Get()->Add("Dead", "Sounds/Dead.wav");

	// UI
	Audio::Get()->Add("UI", "Sounds/UI.mp3");
	Audio::Get()->Add("Slot", "Sounds/Slot.mp3");
	Audio::Get()->Add("TextBox", "Sounds/TextBox.mp3");
	Audio::Get()->Add("Potion", "Sounds/Potion.wav");

	Audio::Get()->Play("Environment");
}
