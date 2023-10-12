#pragma once

class NPC : public ModelAnimator
{
private :
	enum NPCState
	{
		IDLE,
		MINING,
		FELLING
	};

public :
	NPC();
	~NPC();

	void Update();
	void Render();
	void GUIRender();

	void SetWeapon(int idx);
	void SetEvent(float time);
	void SetSound(string name);
	void SetPlay(bool isPlay);
	void AddQuest(string context);
	string GetQuest();

	Collider* GetCollider() { return collider; }

private :
	void PlaySound();
	void SetState(NPCState state);

private :
	SphereCollider* collider;

	NPCState curState;

	// 손 위치 찾기
	Transform* leftHand;
	Transform* rightHand;

	Model* weapon;

	string audioName;
	bool isPlay = false;

	vector<string> quests;
	UINT questNum = 0;
};