#pragma once

class Axe : public Model
{
public:
	Axe(string name);
	~Axe();

	void Update();
	void Render();
	void GUIRender();

	Collider* GetCollider() { return collider; }

private:
	BoxCollider* collider;
};