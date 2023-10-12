#include "Framework.h"

Knight::Knight() : ModelAnimator("Knight")
{
	Create();
}

Knight::~Knight()
{
	Delete();
}

void Knight::Update()
{
	if (Environment::Get()->isFreeMode)
		return;

	if (!UIManager::Get()->InventoryIsShow())
	{
		Move();
		Rotate();
		Jump();
		Attack();
		Communication();
		if (KEY_DOWN('1'))
			weapon = SWORD;
		else if (KEY_DOWN('2'))
			weapon = AXE;
		else if (KEY_DOWN('3'))
			weapon = ICEAXE;
	}
	UIControl();


	ModelAnimator::Update();
	rightHand->SetWorld(GetTransformByNode(38));
	sword->Update();
	axe->Update();
	iceAxe->Update();
	collider->transform->UpdateWorld();
}

void Knight::Render()
{
	ModelAnimator::Render();
	SelectWeapon();
	collider->Render();
}

void Knight::GUIRender()
{
	ModelAnimator::GUIRender();
	sword->GUIRender();
	axe->GUIRender();
	iceAxe->GUIRender();
	collider->transform->GUIRender();
}

void Knight::FloorCollision(Collider* col)
{
	if (isFloor) return;
	if (jumpSpeed > 0) return;

	if (col->IsCollision(collider))
	{
		if (transform->Pos().y < col->transform->GlobalPos().y)
		{
			transform->Pos().y = col->transform->GlobalPos().y;
			isFloor = true;
		}
	}
}

void Knight::SetHit()
{
	isHit = true;
}

void Knight::Attacked(float damage)
{
	if (!isHit) return;
	if (!isAttack)
		SetState(HIT);

	Audio::Get()->Play("Hit");
	curHP -= damage;
	float amount = curHP / MAX_HP;
	UIManager::Get()->GetHPBar()->SetAmount(amount);
}

void Knight::Move()
{
	Vector3 dir;
	if (KEY_PRESS('W'))
	{
		dir += transform->Back();
		if (!isJump && !isAttack && curState != HIT)
			SetState(RUN_F);
	}
	if (KEY_PRESS('S'))
	{
		dir += transform->Forward();
		if(!isJump && !isAttack && curState != HIT)
			SetState(RUN_B);
	}
	if (KEY_PRESS('A'))
	{
		dir += transform->Right();
	}
	if (KEY_PRESS('D'))
	{
		dir += transform->Left();
	}
	if(dir.Length() > 0)
	{
		transform->Pos() += dir * moveSpeed * DELTA;
	}
	else
	{
		if (!isJump && !isAttack && curState != HIT)
			SetState(IDLE);
		//transform->Rot().y = 0;
	}
}

void Knight::Rotate()
{
	Vector3 delta = mousePos - Vector3(CENTER_X, CENTER_Y);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
	
	transform->Rot().y += delta.x * rotSpeed * DELTA;
	CAM->transform->Rot().x -= delta.y * rotSpeed * DELTA;
	//CAM->LookAtTarget();
}

void Knight::Jump()
{
	if (isFloor)
		isJump = false;

	if(isJump || !isFloor)
	{
		jumpSpeed -= GRAVITY * DELTA;
	}
	else
	{
		if (KEY_DOWN(VK_SPACE))
		{
			jumpSpeed = jumpPower;
			isJump = true;
			SetState(JUMP);
			Audio::Get()->Play("Jump");
		}
	}
	transform->Pos().y += jumpSpeed * DELTA;

	if (terrain)
	{
		float h = terrain->GetHeight(transform->Pos());
		if (transform->Pos().y < h)
		{
			transform->Pos().y = h;
			isFloor = true;
		}
		else
		{
			isFloor = false;
		}
	}

}

void Knight::Attack()
{
	if (KEY_DOWN(MK_LBUTTON))
	{
		if (!isAttack)
		{
			isAttack = true;
			SetState(ATTACK);
		}
		else
		{
			isDoubleAttack = true;
		}
	}
	if(isAttack)
		monsterManager->HitMonsterCheck(sword->GetCollider());
}

void Knight::Communication()
{
}

void Knight::SelectWeapon()
{
	switch (weapon)
	{
	case Knight::SWORD:
		sword->Render();
		break;
	case Knight::AXE:
		axe->Render();
		break;
	case Knight::ICEAXE:
		iceAxe->Render();
		break;
	default:
		break;
	}
}

void Knight::UIControl()
{
	if (KEY_DOWN(VK_TAB))
	{
		UIManager::Get()->InventoryIsShow() = !UIManager::Get()->InventoryIsShow();
		Audio::Get()->Play("UI");
	}
	if (KEY_DOWN('4'))
	{
		DrinkPotion();
		Audio::Get()->Play("Potion");
	}
}

void Knight::DrinkPotion()
{
	curHP += 20.0f;
	if (curHP > MAX_HP)
		curHP = MAX_HP;

	float amount = curHP / MAX_HP;
	UIManager::Get()->GetHPBar()->SetAmount(amount);
	UIManager::Get()->GetInventory()->UseItem("Apple");
}

void Knight::SetIdle()
{
	isAttack = false;
	SetState(IDLE);
}

void Knight::EndAttack()
{
	switch (weapon)
	{
	case Knight::SWORD:
		monsterManager->AttackedMonster();
		Audio::Get()->Play("Sword");
		break;
	case Knight::AXE:
		if (treeManager->AxeCollision(GetAxeCollider()))
			UIManager::Get()->GetInventory()->AddItem("Log");
		Audio::Get()->Play("Wood");
		break;
	case Knight::ICEAXE:
		if (rockManager->IceAxeCollision(GetIceAxeCollider()))
			UIManager::Get()->GetInventory()->AddItem("Stone");
		Audio::Get()->Play("Stone");
		break;
	default:
		break;
	}
	if (isDoubleAttack)
	{
		isDoubleAttack = false;
		SetState(ATTACK2);
	}

}

void Knight::SetState(CharacterState state)
{
	if (curState == state) return;

	curState = state;
	if (curState == ATTACK || curState == ATTACK2)
		PlayClip(state, 2.0f);
	else
		PlayClip(state);
}

void Knight::Create()
{
	ClientToScreen(hWnd, &clientCenterPos);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);

	name = "Knight";
	transform->SetTag("Player");
	transform->Load();
	ReadClip("Idle");
	ReadClip("Run_Forward");
	ReadClip("Run_Back");
	ReadClip("Jump");
	ReadClip("Attack");
	ReadClip("Attack2");
	ReadClip("Hit");

	GetClip(JUMP)->SetEvent(bind(&Knight::SetIdle, this), 0.8f);
	GetClip(ATTACK)->SetEvent(bind(&Knight::EndAttack, this), 0.5f);
	GetClip(ATTACK)->SetEvent(bind(&Knight::SetIdle, this), 0.8f);
	GetClip(ATTACK2)->SetEvent(bind(&Knight::EndAttack, this), 0.5f);
	GetClip(ATTACK2)->SetEvent(bind(&Knight::SetIdle, this), 0.8f);
	GetClip(HIT)->SetEvent(bind(&Knight::SetIdle, this), 0.6f);
	//SetEvent(ATTACK, bind(&Knight::SetIdle, this), 0.9f);

	collider = new CapsuleCollider();
	collider->transform->SetParent(transform);
	collider->transform->SetTag("PlayerCollider");
	collider->transform->Load();

	rightHand = new Transform();

	sword = new Sword("Sword");
	sword->transform->SetParent(rightHand);
	sword->transform->Load();

	axe = new Axe("Axe");
	axe->transform->SetParent(rightHand);
	axe->transform->SetTag("PlayerAxe");
	axe->transform->Load();

	iceAxe = new IceAxe("IceAxe");
	iceAxe->transform->SetParent(rightHand);
	iceAxe->transform->SetTag("PlayerIceAxe");
	iceAxe->transform->Load();

	UIManager::Get()->GetHPBar()->SetAmount(1.0f);
}

void Knight::Delete()
{
	delete sword;
	delete axe;
	delete iceAxe;
	delete collider;
	delete rightHand;
}
