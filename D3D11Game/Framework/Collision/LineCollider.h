#pragma once

class LineCollider
{
public:
	LineCollider(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), Vector3 dir = Vector3(0.0f,0.0f,10.0f), float distance = 10.0f);
	~LineCollider();

	void Render();

	void SetLine(Vector3 pos, Vector3 dir);
	void SetDistance(float dist) { distance = dist; }

private:
	void MakeMesh();

private:
	MatrixBuffer* world;

	Mesh<VertexColor>* mesh;

	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;

	Vector3 position;
	Vector3 direction;
	float distance;
};