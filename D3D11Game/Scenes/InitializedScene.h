#pragma once

class InitializedScene : public Scene
{
private :
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT4 color;
		Vertex()
		{
			position = { 0.0f, 0.0f, 0.0f };
			color = { 1, 1, 1, 1 };
		}

		Vertex(float x, float y, float z, float r, float g, float b, float a = 1.0f)
		{
			position = { x, y, z };
			color = { r, g, b, a };
		}
	};
public :
	InitializedScene();
	~InitializedScene();

public :
	virtual void Update() override;
	virtual void Render() override;

private :
	void Test();

private :
	//ID3D11Device* device;
	//ID3D11DeviceContext* context;
	//IDXGISwapChain* swapChain;
	//
	//ID3D11RenderTargetView* renderTargetView;
	//ID3D11DepthStencilView* depthStencilView;

	//ID3D11InputLayout* inputLayout;

	//ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer;


	//UINT stride = 0;
	//UINT offset = 0;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;

	vector<Vertex> vertices;
	vector<UINT> indices;

	// Scene을(를) 통해 상속됨
	virtual void PreRender() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};