#pragma once

// 깊이값 처리 담당
class DepthStencilState
{
public :
	DepthStencilState();
	~DepthStencilState();

	void SetDepthStencilState();

	void SetDepthEnable(bool value);

	void DepthWriteMask(D3D11_DEPTH_WRITE_MASK value);

	void Changed();

private :
	D3D11_DEPTH_STENCIL_DESC desc = {};
	ID3D11DepthStencilState* depthStencilState = nullptr;
};