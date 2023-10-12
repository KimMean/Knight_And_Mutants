#pragma once

class Terrain
{
private :
	typedef VertexUVNormalTangent VertexType;

	const float MIN_HEIGHT = -5.0f;
	const float MAX_HEIGHT = +15.0f;

public :
	Terrain();
	~Terrain();

	void Update();
	void Render();

	void SetDepthTexture(Texture* texture);

	float GetHeight(const Vector3& pos, Vector3* normal = nullptr);
	Vector3 GetPosition(const Vector3& pos);

	Vector2 GetTerrainSize() { return Vector2(width, height); }
	Mesh<VertexType>* GetMesh() { return mesh; }
	
	void LoadTerrain();

private :
	void MakeMesh();
	void MakeNormalVector();
	void MakeTangentVector();

public :
	GameObject* gameObject;


private :
	UINT width, height;

	Mesh<VertexType>* mesh;

	Texture* heightMap;
	Texture* alphaMap;
	Texture* sandTexture;
	Texture* grassTexture;
	Texture* soilTexture;
	Texture* depthTexture;

};