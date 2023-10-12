#pragma once

class TerrainEditorScene : public Scene
{

public :
	TerrainEditorScene();
	~TerrainEditorScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

    //Vector3 PickingTerrain();

private :
	TerrainEditor* terrainEditor;
    LineCollider* line;
	SkyBox* skybox;
};