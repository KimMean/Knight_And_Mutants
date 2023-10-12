#pragma once

class GameObject
{
public :
	GameObject();
	~GameObject();

	virtual void Render() {};
	virtual void SetRender();

	//void SetAllShader(wstring shaderFilePath = L"Shaders/VertexShader.hlsl");
	//void SetVertexShader(wstring shaderFilePath = L"Shaders/VertexShader.hlsl");
	//void SetPixelShader(wstring shaderFilePath = L"Shaders/PixelShader.hlsl");

protected :
	void SetBuffer();
	void SetShader();

public :
	Transform* transform;
	Material* material;
	//VertexShader* vertexShader;
	//PixelShader* pixelShader;
	//MatrixBuffer* worldBuffer;
};