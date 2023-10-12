#pragma once

class Water
{
private :
	class WaterBuffer : public ConstBuffer
	{
	private :
		struct WaterData
		{
			Float2 tile = { 10.0f, 10.0f };
			Float2 moveScale = { 0.01f, 0.01f };

			float timeScale;
			float waveScale = 0.5f;
			float alpha = 0.9f;
			float padding;
		};
	public:
		WaterBuffer() : ConstBuffer(&waterData, sizeof(WaterData))
		{
		}
		WaterData& GetWaterData() { return waterData; }
	private :
		WaterData waterData;
	};

public :
	Water();
	~Water();

	void Update();
	void Render();
	void GUIRender();
	void PostRender();

	void SetReflection();

private :
	void MakeMesh();
	void MakeMesh2();

public:
	GameObject* gameObject;

private :
	Vector2 waterSize;

	Mesh<VertexUV>* mesh;
	WaterBuffer* waterBuffer;
	Texture* waterMap;
	Texture* waterNormal;

	float waveScale = 0.1f;
	//Reflection* reflection;

	float curTime;

};