#pragma once

class Rocks : public Model
{
public:
	Rocks(string name);
	~Rocks();

	void Update();
	void Render();
	virtual void RenderInstance(int count) override;
	void GUIRender();

	SphereCollider* GetCollider() { return collider; }

private:
	SphereCollider* collider;
};