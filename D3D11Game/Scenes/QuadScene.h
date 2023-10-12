#pragma once

class QuadScene : public Scene
{
public :
	QuadScene();
	~QuadScene();


	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;

	virtual void Render() override;

private :
	Quad* quad;
	Cube* cube;
	Sphere* sphere;
	BoxCollider* boxCollider;
	SphereCollider* sphereCollider;


	// Scene��(��) ���� ��ӵ�
	virtual void PreRender() override;

	virtual void PostRender() override;

	virtual void GUIRender() override;

};