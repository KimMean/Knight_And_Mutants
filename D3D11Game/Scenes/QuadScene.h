#pragma once

class QuadScene : public Scene
{
public :
	QuadScene();
	~QuadScene();


	// Scene을(를) 통해 상속됨
	virtual void Update() override;

	virtual void Render() override;

private :
	Quad* quad;
	Cube* cube;
	Sphere* sphere;
	BoxCollider* boxCollider;
	SphereCollider* sphereCollider;


	// Scene을(를) 통해 상속됨
	virtual void PreRender() override;

	virtual void PostRender() override;

	virtual void GUIRender() override;

};