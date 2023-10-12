#pragma once

class Sword : public Model
{
public :
	Sword(string name);
	~Sword();

	void Update();
	void Render();
	void GUIRender();

	Collider* GetCollider() { return collider; }

private :
	BoxCollider* collider;
};