#include "Framework.h"

NPCManager::NPCManager()
{
	CreateNPC();
}

NPCManager::~NPCManager()
{
	for (UINT i = 0; i < MAX_NPC; i++)
	{
		delete npcs[i];
	}
}

void NPCManager::Update()
{
	for (UINT i = 0; i < MAX_NPC; i++)
	{
		npcs[i]->Update();
	}
}

void NPCManager::Render()
{
	for (UINT i = 0; i < MAX_NPC; i++)
	{
		npcs[i]->Render();
	}
}

void NPCManager::GUIRender()
{
	for (UINT i = 0; i < MAX_NPC; i++)
	{
		npcs[i]->GUIRender();
	}
}

bool NPCManager::IsCollisionNPC(Collider* collider)
{
	bool isCollision = false;
	for (UINT i = 0; i < MAX_NPC; i++)
	{
		if (npcs[i]->GetCollider()->IsCollision(collider))
		{
			isCollision = true;
			npcs[i]->SetPlay(true);
			if (KEY_DOWN('F'))
			{
				Audio::Get()->Play("UI");
				if (UIManager::Get()->TextBoxIsShow())
				{
					UIManager::Get()->TextBoxIsShow() = false;
				}
				else
				{
					string str = npcs[i]->GetQuest();
					if (logItem)
					{
						UIManager::Get()->GetInventory()->DeleteItem("Log");
						logItem = false;
					}
					if(stoneItem)
					{
						UIManager::Get()->GetInventory()->DeleteItem("Stone");
						stoneItem = false;
					}
					if (i == 1)
					{
						UIManager::Get()->GetInventory()->AddItem("IceAxe");
						stoneItem = true;
					}
					else if (i == 2)
					{
						UIManager::Get()->GetInventory()->AddItem("Axe");
						logItem = true;
					}
					if (str != "")
					{
						UIManager::Get()->TextBoxIsShow() = true;
						UIManager::Get()->GetTextBox()->SetText(str);
						Audio::Get()->Play("TextBox");
					}
				}
			}
			break;
		}
		else
		{
			npcs[i]->SetPlay(false);
		}
	}
	return isCollision;
}

void NPCManager::CreateNPC()
{
	npcs.resize(MAX_NPC);
	npcs[0] = new NPC();
	npcs[0]->transform->SetTag("Ham_NPC");
	npcs[0]->transform->Load();
	npcs[0]->ReadClip("Hammering");
	npcs[0]->PlayClip(0);
	npcs[0]->SetEvent(0.18f);
	npcs[0]->SetEvent(0.36f);
	npcs[0]->SetEvent(0.54f);
	npcs[0]->SetEvent(0.8f);
	npcs[0]->SetWeapon(0);
	npcs[0]->SetSound("Hammer");
	npcs[0]->AddQuest("안녕하세요 모험자님,\n 부탁 하나만 들어주셨으면 합니다.\n주변에서 벌목을 하고 있는 Fell을 도와 목재 10개만 부탁드리겠습니다.");
	npcs[0]->AddQuest("목재를 구해다 주셨군요, 감사합니다.\n 혹시 부탁 하나만 더 드려도 되겠습니까?");
	npcs[0]->AddQuest("숲속에서 광석을 캐고 있는 Mine을 도와 못의 재료가 되는 광석을 10개만 구해다 주십시오.");
	npcs[0]->AddQuest("모험자님 큰일 났습니다.\n주변에 몬스터 무리가 나타났습니다.\n 마을의 안전을 위해 몬스터 토벌을 부탁드립니다.");

	npcs[1] = new NPC();
	npcs[1]->transform->SetTag("Mine_NPC");
	npcs[1]->transform->Load();
	npcs[1]->ReadClip("Mining");
	npcs[1]->PlayClip(0);
	npcs[1]->SetEvent(0.4f);
	npcs[1]->SetWeapon(1);
	npcs[1]->SetSound("Stone");
	npcs[1]->AddQuest("안녕하세요 모험자님, 도와주셔서 감사합니다.\n채광에 필요한 곡괭이는 가방에 넣어뒀습니다.");


	npcs[2] = new NPC();
	npcs[2]->transform->SetTag("Fell_NPC");
	npcs[2]->transform->Load();
	npcs[2]->ReadClip("Felling");
	npcs[2]->PlayClip(0);
	npcs[2]->SetEvent(0.35f);
	npcs[2]->SetWeapon(2);
	npcs[2]->SetSound("Wood");
	npcs[2]->AddQuest("안녕하세요 모험자님, 도와주셔서 감사합니다.\n 벌목에 필요한 도끼는 가방에 넣어뒀습니다.");

}
