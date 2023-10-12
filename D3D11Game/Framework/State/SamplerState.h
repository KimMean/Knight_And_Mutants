#pragma once

// SamplerState는 텍스처의 특정 위치에서 픽셀 값을 읽어오는 방법과 방식을 지정합니다.
class SamplerState
{
public :
	SamplerState();
	~SamplerState();

	void SetState(UINT slot = 0);

	void Filter(D3D11_FILTER filter);

	void Changed();

private :
	D3D11_SAMPLER_DESC desc = {};
	ID3D11SamplerState* state = nullptr;
};