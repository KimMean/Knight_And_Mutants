#pragma once

class RockManager : public ModelInstancingManager
{
public:
	RockManager(Model* model, string name, UINT count);
	~RockManager();

	void Update();
	void Render();
	void GUIRender();

	void PushCollider(Collider* collider);
	bool IceAxeCollision(Collider* collider);

private:
	vector<Rocks*> rocks;
};