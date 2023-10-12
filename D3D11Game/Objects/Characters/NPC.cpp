#include "Framework.h"

NPC::NPC() : ModelAnimator("NPC")
{
	
	collider = new SphereCollider(300);
	collider->transform->SetParent(transform);
	collider->transform->Pos().y = 150.0f;

	//ReadClip("Hammering");
	//ReadClip("Mining");
	//ReadClip("Felling");

	//GetClip(0)->SetEvent(bind(&NPC::PlaySound, this), 0.2f);
	//GetClip(0)->SetEvent(bind(&NPC::PlaySound, this), 0.8f);
	//GetClip(1)->SetEvent(bind(&NPC::PlaySound, this), 0.8f);
	//GetClip(2)->SetEvent(bind(&NPC::PlaySound, this), 0.8f);


	leftHand = new Transform();
	rightHand = new Transform();
}

NPC::~NPC()
{
	delete collider;
	delete leftHand;
	delete rightHand;
}

void NPC::Update()
{
	ModelAnimator::Update();
	leftHand->SetWorld(GetTransformByNode(10));
	rightHand->SetWorld(GetTransformByNode(31));
	weapon->transform->UpdateWorld();
	collider->transform->UpdateWorld();
}

void NPC::Render()
{
	ModelAnimator::Render();
	weapon->Render();
	collider->Render();
}

void NPC::GUIRender()
{
	ModelAnimator::GUIRender();
	weapon->GUIRender();

	//collider->Render();
}

void NPC::SetWeapon(int idx)
{
	switch (idx)
	{
	case 0 :
		weapon = new Hammer("Hammer");
		break;
	case 1:
		weapon = new IceAxe("IceAxe");
		weapon->transform->Load();
		break;
	case 2:
		weapon = new Axe("Axe");
		weapon->transform->Load();
		break;
	}
	weapon->transform->SetParent(rightHand);
}

void NPC::SetEvent(float time)
{
	GetClip(0)->SetEvent(bind(&NPC::PlaySound, this), time);
}

void NPC::SetSound(string name)
{
	audioName = name;
}

void NPC::SetPlay(bool isPlay)
{
	this->isPlay = isPlay;
}

void NPC::AddQuest(string context)
{
	quests.push_back(context);
}

string NPC::GetQuest()
{
	if (quests.size() > questNum)
	{
		return quests[questNum++];
	}
	return "";
}

void NPC::PlaySound()
{
	if (isPlay)
	{
		if (Audio::Get()->IsPlaySound(audioName))
			Audio::Get()->Stop(audioName);
		Audio::Get()->Play(audioName);
	}
}

void NPC::SetState(NPCState state)
{
}
