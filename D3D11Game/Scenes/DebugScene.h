#pragma once

class DebugScene : public Scene
{
public :
	DebugScene();
	~DebugScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private :
	bool isWireFrameMode = false;
};