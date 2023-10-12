#pragma once

class TreeManager : public ModelInstancingManager
{
public :
	TreeManager(Model* model, string name, UINT count);
	~TreeManager();

	void Update();
	void Render();
	void GUIRender();

	void PushCollider(Collider* collider);
	bool AxeCollision(Collider* collider);

private :
	vector<Tree*> trees;
};