#pragma once

class MonsterManager
{
private :
	const UINT MONSTER_COUNT = 10;
	const float SPAWN_TIME = 5.0f;

public :
	MonsterManager();
	~MonsterManager();

	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void GUIRender();

	void HitMonsterCheck(Collider* collider);
	void AttackedMonster();
	void TrackingPlayer(Collider* collider);

public :
	void SetShader(wstring shaderFile);
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetAStar(AStar* astar) { this->astar = astar; }
	void SetTarget(Knight* target);
	void Spawn();

private :
	ModelAnimatorInstancing* monsterInstancing;
	vector<Mutant*> monsters;

	Terrain* terrain;
	AStar* astar;
	Knight* target;
	float spawnTime = 0.0f;

	int monsterID = 0;
	int tempClip = 0;
};