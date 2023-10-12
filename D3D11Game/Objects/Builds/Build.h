#pragma once

class Build : public Model
{
public :
	Build(string name);
	~Build();

	void Update();
	void Render();
	void GUIRender();

	BoxCollider* GetCollider() { return collider; }
	BoxCollider* GetCollider(UINT i) { return stairs[i]; }
	void PushCollider(Collider* collider);

private :
	BoxCollider* collider;
	vector<BoxCollider*> stairs;

};