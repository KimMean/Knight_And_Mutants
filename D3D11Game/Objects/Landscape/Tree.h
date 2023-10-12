#pragma once

class Tree : public Model
{
public :
	Tree(string name);
	~Tree();

	void Update();
	void Render();
	virtual void RenderInstance(int count) override;
	void GUIRender();

	CapsuleCollider* GetCollider() { return collider; }

private :
	CapsuleCollider* collider;

};