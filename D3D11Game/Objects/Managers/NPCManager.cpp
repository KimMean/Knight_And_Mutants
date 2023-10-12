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
	npcs[0]->AddQuest("�ȳ��ϼ��� �����ڴ�,\n ��Ź �ϳ��� ����ּ����� �մϴ�.\n�ֺ����� ������ �ϰ� �ִ� Fell�� ���� ���� 10���� ��Ź�帮�ڽ��ϴ�.");
	npcs[0]->AddQuest("���縦 ���ش� �ּ̱���, �����մϴ�.\n Ȥ�� ��Ź �ϳ��� �� ����� �ǰڽ��ϱ�?");
	npcs[0]->AddQuest("���ӿ��� ������ ĳ�� �ִ� Mine�� ���� ���� ��ᰡ �Ǵ� ������ 10���� ���ش� �ֽʽÿ�.");
	npcs[0]->AddQuest("�����ڴ� ū�� �����ϴ�.\n�ֺ��� ���� ������ ��Ÿ�����ϴ�.\n ������ ������ ���� ���� ����� ��Ź�帳�ϴ�.");

	npcs[1] = new NPC();
	npcs[1]->transform->SetTag("Mine_NPC");
	npcs[1]->transform->Load();
	npcs[1]->ReadClip("Mining");
	npcs[1]->PlayClip(0);
	npcs[1]->SetEvent(0.4f);
	npcs[1]->SetWeapon(1);
	npcs[1]->SetSound("Stone");
	npcs[1]->AddQuest("�ȳ��ϼ��� �����ڴ�, �����ּż� �����մϴ�.\nä���� �ʿ��� ��̴� ���濡 �־�׽��ϴ�.");


	npcs[2] = new NPC();
	npcs[2]->transform->SetTag("Fell_NPC");
	npcs[2]->transform->Load();
	npcs[2]->ReadClip("Felling");
	npcs[2]->PlayClip(0);
	npcs[2]->SetEvent(0.35f);
	npcs[2]->SetWeapon(2);
	npcs[2]->SetSound("Wood");
	npcs[2]->AddQuest("�ȳ��ϼ��� �����ڴ�, �����ּż� �����մϴ�.\n ���� �ʿ��� ������ ���濡 �־�׽��ϴ�.");

}
