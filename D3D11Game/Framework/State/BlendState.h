#pragma once

// ºí·»µù ´ã´ç
class BlendState
{
public:
	BlendState();
	~BlendState();

	void SetBlendState();

	void SetAlphaBlend(bool value);
	void SetAdditiveBlend();
	void SetAlphaToCoverage(bool value);

	void Changed();
private:
	D3D11_BLEND_DESC desc = {};
	ID3D11BlendState* blendState = nullptr;
};
