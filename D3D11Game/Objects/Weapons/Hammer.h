#pragma once

class Hammer : public Model
{
public:
	Hammer(string name);
	~Hammer();

	void Update();
	void Render();
	void GUIRender();

private:
	BoxCollider* collider;
};