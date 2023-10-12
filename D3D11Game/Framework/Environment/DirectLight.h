#pragma once

class DirectLight
{
private :
	struct DirectionalLight
	{
		Float4 color = COLOR_WHITE;
		Float3 direction = { 1, -1, 1 };
		float intensity = 1.0f;

		Float3 pos = { 0, 100.0f, 0 };
		float range = 100.0f;
	};

	//struct Data
	//{
	//	DirectionalLight lights[MAX_LIGHT];
	//
	//	UINT lightCount = 1;
	//	Float3 ambientLight = { 0.1f, 0.1f, 0.1f };
	//	Float3 ambientCeil = { 0.1f, 0.1f, 0.1f };
	//	float padding;
	//};

public :
	DirectLight();
	~DirectLight();

	void GUIRender();

	DirectionalLight& GetLight() { return light; };
	void SetPixelShaderBuffer(UINT slot);

private :
	ConstBuffer* constBuffer;

	DirectionalLight light;

	//Float4 color = COLOR_WHITE;
	//Float3 direction = { 0, -1, 1 };
	//float intensity = 1.0f;
};