#pragma once

class GameScene : public Scene
{
public :
	GameScene();
	~GameScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private :
	void CollisionCheck();

	void Create();
	void Delete();

	void CreateSound();

private :
	AStar* aStar;
	SkyBox* skyBox;
	Water* water;
	Terrain* terrain;
	Build* house;
	Knight* player;
	NPCManager* npcManager;
	//NPC* npc;
	//Monster* monster;

	TreeManager* treeManager;
	RockManager* rockManager;
	//ModelInstancingManager* treeInstancing;
	//ModelInstancingManager* rockInstancing;

	//Model* test;
	MonsterManager* monsterManager;

	//Shadow* shadow;

};