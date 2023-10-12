#pragma once

class Knight : public ModelAnimator
{
private :
	enum CharacterState
	{
		IDLE,
		RUN_F, 
		RUN_B,
		JUMP,
		ATTACK,
		ATTACK2,
		HIT
	};

	enum Weapon
	{
		SWORD,
		AXE,
		ICEAXE
	};
	const float MAX_HP = 100.0f;

public :
	Knight();
	~Knight();

	void Update();
	void Render();
	void GUIRender();

	void SetTerrain(class Terrain* terrain) { this->terrain = terrain; }
	void SetTreeManager(class TreeManager* trees) { treeManager = trees; }
	void SetRockManager(class RockManager* rocks) { rockManager = rocks; }
	void SetMonsterManager(class MonsterManager* monsters) { monsterManager = monsters; }

	CapsuleCollider* GetCollider() { return collider; }
	Collider* GetAxeCollider() { return axe->GetCollider(); }
	Collider* GetIceAxeCollider() { return iceAxe->GetCollider(); }

	void FloorCollision(Collider* col);

	void SetHit();
	void Attacked(float damage);

private :
	void Move();
	void Rotate();
	void Jump();
	void Attack();
	void Communication();
	void SelectWeapon();
	void UIControl();

	void DrinkPotion();

	void SetIdle();
	void EndAttack();
	void SetState(CharacterState state);

	void Create();
	void Delete();


	//void SetEvent(int clip, Event event, float timeRatio);
	//void ExcuteEvent();

private :
	Sword* sword;
	Axe* axe;
	IceAxe* iceAxe;
	Transform* rightHand;
	Weapon weapon = SWORD;

	CapsuleCollider* collider;

	class Terrain* terrain = nullptr;
	class TreeManager* treeManager = nullptr;
	class RockManager* rockManager = nullptr;
	class MonsterManager* monsterManager = nullptr;

	CharacterState curState = IDLE;

	float curHP = MAX_HP;
	float moveSpeed = 2.0f;
	float rotSpeed = 1.0f;
	float jumpPower = 5.0f;
	float jumpSpeed = 0.0f;

	bool isFloor = false;
	bool isJump = false;
	bool isAttack = false;
	bool isHit = false;
	bool isDoubleAttack = false;

	//ModelAnimator* anim;

	//ModelAnimatorInstancing::Motion* motion;
	//vector<map<float, Event>> totalEvent;
	//vector<map<float, Event>::iterator> eventIters;

	POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };
};