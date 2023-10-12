#include "Framework.h"

Mutant::Mutant(Transform* transform, ModelAnimatorInstancing* instancing, UINT index)
    : transform(transform), instancing(instancing), index(index)
{
    root = new Transform();
    leftHand = new Transform();

    collider = new CapsuleCollider(50, 100);
    collider->transform->SetParent(root);

    handCollider = new BoxCollider(Vector3(50,120,50));
    handCollider->transform->SetParent(leftHand);
    handCollider->transform->Pos() = { -15.0f, 80.0f, -10.0f };

    trackingCollider = new SphereCollider(800.0f);
    trackingCollider->transform->SetParent(this->transform);
    trackingCollider->transform->Pos().y = 100.0f;

    //hpBar = new ProgressBar(Vector2(100, 10));
    //hpBar->SetTexture(L"Textures/Icons/hp_bar_BG.png");
    //hpBar->SetFrontImage(Vector2(100, 10), L"Textures/Icons/hp_bar.png");
    //hpBar->SetAmount(0.5f);
    hpBar = new ProgressBar(Vector2(1.0f, 0.1f));
    hpBar->SetTexture(L"Textures/Icons/hp_bar_BG.png");
    hpBar->SetFrontImage(Vector2(1.0f, 0.1f), L"Textures/Icons/hp_bar.png");
    //hpBar->SetAmount(0.5f);
    //handCollider->transform->Rot() = 

    motion = instancing->GetMotion(index);
    totalEvent.resize(instancing->GetClipSize());
    eventIters.resize(instancing->GetClipSize());

    SetEvent(ROAR, bind(&Mutant::RoarSound, this), 0.3f);
    SetEvent(ROAR, bind(&Mutant::EndRoar, this), 0.9f);
    SetEvent(ATTACK, bind(&Mutant::AttackSound, this), 0.4f);
    SetEvent(ATTACK, bind(&Mutant::EndAttack, this), 0.9f);
    SetEvent(ATTACK, bind(&Mutant::TargetHitCheck, this), 0.5f);
    SetEvent(DEATH, bind(&Mutant::EndDeath, this), 0.9f);
    //SetEvent(HIT, bind(&Robot::EndHit, this), 0.9f);
    //SetEvent(DYING, bind(&Robot::EndDying, this), 0.9f);

    for(UINT i = 0; i < totalEvent.size(); i++)
    {
        eventIters[i] = totalEvent[i].begin();
    }

    //hpBar = new ProgressBar(L"Textures/UI/hp_bar.png",
    //    L"Textures/UI/hp_bar_BG.png");
}

Mutant::~Mutant()
{
    delete root;
    delete leftHand;
    delete collider;
    delete handCollider;
    delete trackingCollider;
    delete hpBar;
}

void Mutant::Update()
{

    ExcuteEvent();
    BehaviorTime();
    Control();
    UpdateHpBar();

    root->SetWorld(instancing->GetTransformByNode(index, 2));
    leftHand->SetWorld(instancing->GetTransformByNode(index, 12));
    collider->transform->UpdateWorld();
    handCollider->transform->UpdateWorld();
    trackingCollider->transform->UpdateWorld();

}

void Mutant::Render()
{
    collider->Render();
    handCollider->Render();
    trackingCollider->Render();

    hpBar->SetRender();
    hpBar->frontImage->Render();
    hpBar->Render();
}

void Mutant::PostRender()
{
    //if (!CAM->ContainPoint(barPos)) return;
    //CAM->ScreenPointToRay(barPos)
    
    //hpBar->Render();
}

void Mutant::GUIRender()
{
    //handCollider->transform->GUIRender();
    //transform->GUIRender();
    //if (!CAM->ContainPoint(barPos)) return;
    //hpBar->Render();
}

void Mutant::Control()
{
    if(target)
        velocity = target->transform->GlobalPos() - transform->GlobalPos();
    switch (curState)
    {
    case Mutant::IDLE:
        break;
    case Mutant::WALK:
        Move();
        break;
    case Mutant::ROAR:
        break;
    case Mutant::ATTACK:
        Attack();
        break;
    case Mutant::DEATH:
        break;
    default:
        break;
    }

    //Move();
}

void Mutant::BehaviorTime()
{
    if (curState != IDLE) 
        return;

    curTime += DELTA;

    // 일정 시간 후 이동
    if (curTime > stateChangeTime)
    {
        isMoving = true;

        curTime -= stateChangeTime;
        stateChangeTime = Random(3.0f, 7.0f);

        // Walk Path
        int s = aStar->FindCloseNode(transform->Pos());
        int e = aStar->FindRandomPos(transform->Pos(), 10.0f);
        aStar->GetPath(s, e, path);
        
        SetState(WALK);
    }
}

void Mutant::Move()
{
    if (path.empty())
    {
        isMoving = false;
        SetIdle();
        return;
    }

    Vector3 dest = path.back();

    Vector3 direction = dest - transform->GlobalPos();
    direction.y = 0.0f;

    if (direction.Length() < 1.0f)
        path.pop_back();

    Vector3 velocity = direction.GetNormalized();

    transform->Pos() += velocity * moveSpeed * DELTA;
    transform->Pos().y = terrain->GetHeight(transform->Pos());
    transform->Rot().y = atan2(velocity.x, velocity.z) + XM_PI;

    if (isTracking)
    {
        // 추적 중 경로는 계속 변경, 1초마다 변경하면 더 좋을 듯
        int startIndex = aStar->FindCloseNode(transform->Pos());
        int endIndex = aStar->FindCloseNode(target->transform->Pos());
        aStar->GetPath(startIndex, endIndex, path);

        // 플레이어와 몬스터의 거리가 일정 거리 이하일 때 추적 중지하고 공격
        Vector3 dir = transform->GlobalPos() - target->transform->GlobalPos();

        if (dir.Length() < 1.0f)
        {
            path.clear();
            SetState(ATTACK);
            isAttack = true;
        }
    }

}

void Mutant::Attack()
{
    // 플레이어와 왼손 충돌 체크
    // 공격이 끝났을 때 플레이어 데미지 쳌
    if (!isAttack) return;

    if (handCollider->IsCollision(target->GetCollider()))
    {
        target->SetHit();
    }
}

void Mutant::SetHit()
{
    isHit = true;
}

void Mutant::Attacked(float damage)
{
    if (!isHit) return;

    isHit = false;
    curHp -= damage;
    Audio::Get()->Play("MHit");
    if (curHp <= 0)
    {
        curHp = 0.0f;
        SetState(DEATH);
        Audio::Get()->Play("Dead");
    }
}

void Mutant::TrackingPlayer(bool tracking)
{
    if (isTracking == tracking) return;

    isTracking = tracking;
    if(isTracking)
    {
        SetState(ROAR);
    }
}

void Mutant::Spawn()
{
    transform->Pos() = aStar->FindRandomPos();
    transform->UpdateWorld();
    transform->SetActive(true);
    stateChangeTime = Random(3.0f, 7.0f);
}


//void Mutant::Spawn(Vector3 pos)
//{
//    //transform->SetActive(true);
//    //collider->transform->SetActive(true);
//    //transform->Pos() = pos;
//}


void Mutant::SetEvent(int clip, Event event, float timeRatio)
{
    if (totalEvent[clip].count(timeRatio) > 0)
        return;

    totalEvent[clip][timeRatio] = event;
}

void Mutant::ExcuteEvent()
{
    int index = curState;

    if (totalEvent[index].empty()) return;
    if (eventIters[index] == totalEvent[index].end()) return;

    float ratio = motion->runningTime / motion->duration;

    if (eventIters[index]->first > ratio) return;

    eventIters[index]->second();
    eventIters[index]++;
}

void Mutant::SetIdle()
{
    SetState(IDLE);
    curTime = 0.0f;
}

void Mutant::RoarSound()
{
    Audio::Get()->Play("Roar");
}

void Mutant::EndRoar()
{
    // 울부짖은 후 경로설정해서 움직임
    int startIndex = aStar->FindCloseNode(transform->Pos());
    int endIndex = aStar->FindCloseNode(target->transform->Pos());
    aStar->GetPath(startIndex, endIndex, path);

    moveSpeed = 2.0f;

    SetState(WALK);
}

void Mutant::TargetHitCheck()
{
    target->Attacked(10.0f);
    isAttack = false;
}

void Mutant::AttackSound()
{
    Audio::Get()->Play("MAttack");
}

void Mutant::EndAttack()
{
    isTracking = false;
    moveSpeed = 1.0f;
    SetState(IDLE);
}

void Mutant::EndDeath()
{
    UIManager::Get()->GetInventory()->AddItem("Apple");
    transform->SetActive(false);
}

void Mutant::SetState(MonsterState state)
{
    if (curState == state) return;

    curState = state;
    instancing->PlayClip(index, state);
    eventIters[state] = totalEvent[state].begin();
}

void Mutant::UpdateHpBar()
{
    // hpBar 위치 및 회전
    Vector3 p = transform->Pos() + Vector3(0, 1.0f, 0);
    Vector3 dir = (transform->Pos() - CAM->transform->Pos()).GetNormalized();
    Vector3 forward = Vector3(dir.x, 0.0f, dir.z).GetNormalized();

    float x = acos(Dot(forward, dir));
    float y = atan2(dir.x, dir.z);

    hpBar->gameObject->transform->Pos() = p;
    hpBar->gameObject->transform->Rot().x = x;
    hpBar->gameObject->transform->Rot().y = y;

    float ratio = curHp / MAX_HP;
    hpBar->SetAmount(ratio);
    hpBar->Update();
    //barPos = transform->Pos() + Vector3(0, 2.0f, 0);
    //
    ////hpBar->gameObject->transform->Pos() = barPos;
    //hpBar->gameObject->transform->Pos() = CAM->WorldToScreen(barPos);
    //float scale = 100.0f / velocity.Length();
    //scale = Clamp(0.1f, 1.0f, scale);
    //hpBar->gameObject->transform->Scale() = { scale, scale, scale };
    //hpBar->gameObject->transform->UpdateWorld();
}
