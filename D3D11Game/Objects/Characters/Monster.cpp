#include "Framework.h"

Monster::Monster() : ModelAnimator("Knight")
{
	transform->SetTag("Monster");
	ReadClip("Idle");
	collider = new CapsuleCollider();
	collider->transform->SetParent(transform);
}

Monster::~Monster()
{
}

void Monster::Update()
{
	ModelAnimator::Update();
	collider->transform->UpdateWorld();
}

void Monster::Render()
{
	ModelAnimator::Render();
	collider->Render();
}

void Monster::GUIRender()
{
	ModelAnimator::GUIRender();
}

void Monster::Attacked(float damage)
{
}

void Monster::SetState(MonsterState state)
{
}
