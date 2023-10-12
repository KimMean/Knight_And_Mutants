#pragma once

class GridScene : public Scene
{
private :
	const int GRID_SIZE = 100;
	const int GRID_OFFSET = 10;

public :
	GridScene();
	~GridScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private :
	void MakeMesh();

private :
	Transform* transform;

	Mesh<VertexColor>* mesh;

	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;

	MatrixBuffer* worldBuffer;


};