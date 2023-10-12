#include "Framework.h"

MonsterManager::MonsterManager()
{
	monsterInstancing = new ModelAnimatorInstancing("Mutant");
	monsterInstancing->ReadClip("Idle");
	monsterInstancing->ReadClip("Walk");
	monsterInstancing->ReadClip("Roar");
	monsterInstancing->ReadClip("Attack");
	monsterInstancing->ReadClip("Death");

}

MonsterManager::~MonsterManager()
{
	delete monsterInstancing;
	for (Mutant* mutant : monsters)
		delete mutant;
}

void MonsterManager::Update()
{
	for (Mutant* mutant : monsters)
	{
		if (!mutant->transform->ActiveSelf()) continue;
		
		mutant->Update();
		// ¸ó½ºÅÍ ³¢¸® ¹Ð¾î³¿
		for (Mutant* m : monsters)
			mutant->GetCollider()->PushCollision(m->GetCollider());
	}

	monsterInstancing->Update();
}

void MonsterManager::PreRender()
{
	monsterInstancing->PreRender();
}

void MonsterManager::Render()
{
	for (Mutant* mutant : monsters)
	{
		if (!mutant->transform->ActiveSelf()) continue;
		mutant->Render();
	}

	monsterInstancing->Render();
}

void MonsterManager::PostRender()
{
	for (Mutant* mutant : monsters)
		mutant->PostRender();
}

void MonsterManager::GUIRender()
{
	ImGui::SliderInt("ID", &monsterID, 0, 10);
	ImGui::SliderInt("Clip", &tempClip, 0, 5);
	
	if (ImGui::Button("Play"))
		monsterInstancing->PlayClip(monsterID, tempClip);

	//monsters[0]->GUIRender();
	//monsters[0]->GUIRender();
	//monsterInstancing->GUIRender();
}

void MonsterManager::HitMonsterCheck(Collider* collider)
{
	for (Mutant* mutant : monsters)
	{
		if (mutant->GetCollider()->IsCollision(collider))
		{
			mutant->SetHit();
		}
	}
}

void MonsterManager::AttackedMonster()
{
	for (Mutant* mutant : monsters)
	{
		mutant->Attacked(10.0f);
	}
}

void MonsterManager::TrackingPlayer(Collider* collider)
{
	for (Mutant* mutant : monsters)
	{
		if (mutant->GetTrackingCollider()->IsCollision(collider))
		{
			mutant->GetTrackingCollider()->SetColor(COLOR_RED);
			mutant->TrackingPlayer(true);
		}
		else
		{
			mutant->TrackingPlayer(false);
			mutant->GetTrackingCollider()->SetColor(COLOR_GREEN);
		}
	}
}

void MonsterManager::SetShader(wstring shaderFile)
{
	monsterInstancing->SetShader(shaderFile);
}

void MonsterManager::SetTarget(Knight* target)
{
	this->target = target;

	for (Mutant* mutant : monsters)
		mutant->SetTarget(target);
}

void MonsterManager::Spawn()
{
	for (UINT i = 0; i < MONSTER_COUNT; i++)
	{
		Transform* transform = monsterInstancing->Add();
		transform->Scale() *= 0.005f;
		Mutant* mutant = new Mutant(transform, monsterInstancing, i);
		mutant->SetTerrain(terrain);
		mutant->SetAStar(astar);
		mutant->SetTarget(target);
		mutant->Spawn();
		monsters.push_back(mutant);
	}
}
