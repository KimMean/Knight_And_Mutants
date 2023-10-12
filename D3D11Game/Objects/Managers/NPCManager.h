#pragma once

class NPCManager
{
private :
	const UINT MAX_NPC = 3;

public :
	NPCManager();
	~NPCManager();

	void Update();
	void Render();
	void GUIRender();

	bool IsCollisionNPC(Collider* collider);

private :
	void CreateNPC();

private :
	vector<NPC*> npcs;
	vector<string> npcText;

	bool logItem = false;
	bool stoneItem = false;
};