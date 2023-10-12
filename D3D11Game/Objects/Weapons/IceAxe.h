#pragma once


class IceAxe : public Model
{
public:
	IceAxe(string name);
	~IceAxe();

	void Update();
	void Render();
	void GUIRender();

	Collider* GetCollider() { return collider; }

private:
	BoxCollider* collider;
};