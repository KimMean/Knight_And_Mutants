#pragma once

class Mutant
{
private:
	const float MAX_HP = 100.0f;
	
	enum MonsterState
	{
		IDLE,
		WALK,
		ROAR,
		ATTACK,
		DEATH
	};

public:
	Mutant(Transform* transform, ModelAnimatorInstancing* instancing, UINT index);
	~Mutant();

	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void Control();
	void BehaviorTime();
	void Move();
	void Attack();

	void SetHit();
	void Attacked(float damage);
	void TrackingPlayer(bool tracking);

	void Spawn();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetAStar(AStar* aStar) { this->aStar = aStar; }
	void SetTarget(Knight* target) { this->target = target; }
	CapsuleCollider* GetCollider() { return collider; }
	SphereCollider* GetTrackingCollider() { return trackingCollider; }

private:

	void SetEvent(int clip, Event event, float timeRatio);
	void ExcuteEvent();

	void SetIdle();
	void RoarSound();
	void EndRoar();
	void TargetHitCheck();
	void AttackSound();
	void EndAttack();
	void EndDeath();
	void SetState(MonsterState state);

	void UpdateHpBar();

public :
	Transform* transform;

private:
	ModelAnimatorInstancing* instancing;		// �ν��Ͻ̰�ü
	ModelAnimatorInstancing::Motion* motion;	// ���
	UINT index;									// �ν��Ͻ� ID

	Transform* root;			// �߽���
	Transform* leftHand;		// �޼�
	CapsuleCollider* collider;	// ��ü
	BoxCollider* handCollider;	// �ճ�
	SphereCollider* trackingCollider;

	Terrain* terrain;			// ����
	AStar* aStar;				// AStar
	vector<Vector3> path;		// ������ ���
	Knight* target;				// Player
	bool isMoving = false;		// �̵� üũ
	bool isTracking = false;	// ���� ���� üũ
	bool isAttack = false;		// ���� üũ
	bool isRoar = false;		// ���� �� �Լ�
	bool isHit = false;			// ���� ����

	MonsterState curState = IDLE;	// ����
	float curTime = 0.0f;			// ���� ���� �ð�
	float stateChangeTime;			// ���� ��ȯ �ð�


	ProgressBar* hpBar;				// ü�¹�
	Vector3 barPos;					// ü�¹� ��ġ
	Vector3 velocity;				// ����
	float curHp = 100.0f;			// hp
	float moveSpeed = 1.0f;			// ������ �ӵ�


	vector<map<float, Event>> totalEvent;			// �ִϸ��̼� �̺�Ʈ
	vector<map<float, Event>::iterator> eventIters;	// �̺�Ʈ ���ͷ�����
};