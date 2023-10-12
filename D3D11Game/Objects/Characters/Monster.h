#pragma once

// �ν��Ͻ� ��� ����
class Monster : public ModelAnimator
{
private :
	const float MAX_HP = 100.0f;

	enum MonsterState
	{
		NONE,
		CREATE,
		WALK,
		RUN,
		ATTACK,
		HIT,
		DYING
	};

public :
	Monster();
	~Monster();
	
	void Update();
	void Render();
	void GUIRender();

	//moveAstar
	// 
	void Attacked(float damage);
	

private :
	void SetState(MonsterState state);

private :
	CapsuleCollider* collider;	// ��ü
	// ���� �ϳ� ���
	

	MonsterState curState = NONE;
	float maxHp = 100.0f;
	float curHp = 100.0f;

	float moveSpeed;


	// hpBar
	// 
};