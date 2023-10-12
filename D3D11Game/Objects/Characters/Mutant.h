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
	ModelAnimatorInstancing* instancing;		// 인스턴싱객체
	ModelAnimatorInstancing::Motion* motion;	// 모션
	UINT index;									// 인스턴싱 ID

	Transform* root;			// 중심점
	Transform* leftHand;		// 왼손
	CapsuleCollider* collider;	// 몸체
	BoxCollider* handCollider;	// 손날
	SphereCollider* trackingCollider;

	Terrain* terrain;			// 지형
	AStar* aStar;				// AStar
	vector<Vector3> path;		// 움직임 경로
	Knight* target;				// Player
	bool isMoving = false;		// 이동 체크
	bool isTracking = false;	// 추적 범위 체크
	bool isAttack = false;		// 공격 체크
	bool isRoar = false;		// 공격 전 함성
	bool isHit = false;			// 공격 받음

	MonsterState curState = IDLE;	// 상태
	float curTime = 0.0f;			// 진행 단위 시간
	float stateChangeTime;			// 상태 전환 시간


	ProgressBar* hpBar;				// 체력바
	Vector3 barPos;					// 체력바 위치
	Vector3 velocity;				// 방향
	float curHp = 100.0f;			// hp
	float moveSpeed = 1.0f;			// 움직임 속도


	vector<map<float, Event>> totalEvent;			// 애니메이션 이벤트
	vector<map<float, Event>::iterator> eventIters;	// 이벤트 이터레이터
};