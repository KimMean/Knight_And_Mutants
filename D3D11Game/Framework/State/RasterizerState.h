#pragma once

// 폴리곤 그리는 방법 담당
class RasterizerState
{
public :
	RasterizerState();
	~RasterizerState();

	void SetRasterizerState();

	void SetCullMode(D3D11_CULL_MODE mode = D3D11_CULL_BACK);
	void SetFillMode(D3D11_FILL_MODE value = D3D11_FILL_SOLID);
	void SetFrontCounterClockwise(bool value);

	void Changed();

private :
	D3D11_RASTERIZER_DESC desc = {};
	ID3D11RasterizerState* rasterizerState = nullptr;
};