#pragma once

// SamplerState�� �ؽ�ó�� Ư�� ��ġ���� �ȼ� ���� �о���� ����� ����� �����մϴ�.
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