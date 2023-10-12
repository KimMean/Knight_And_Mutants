#pragma once

class SkyScene : public Scene
{
public :
	SkyScene();
	~SkyScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private :
	SkyBox* skyBox;
	Water* water;
	//Quad* quad;
	//Sphere* sky;
	//Texture* cubeMap;
};