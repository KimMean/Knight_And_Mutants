#include "Framework.h"

SamplerState::SamplerState()
{
    //�ؽ�ó�� ���ø��� �� ����� ���͸� ����� ����
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    // �ؽ�ó ��ǥ�� ������ ����� �� ó�� ����� �����մϴ�.
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    // �� ������ ������ �� ���Ǵ� ���̾ ���� ����
    //desc.MipLODBias;
    // ���͸��� ����� �� ����Ǵ� �ִ� ǥ��ȭ ���� ����
    //desc.MaxAnisotropy;
    // �ؽ�ó ���� ���� �� ����� �� �Լ��� �����մϴ�.
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    // �ؽ�ó ��ǥ�� ��� ���� ����� �� ����� ���� ���� ����
    //desc.BorderColor[4];
    // ī�޶��� �Ÿ��� ���� ����Ƽ�� �ٸ��� ������ �� ���
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    Changed();
}

SamplerState::~SamplerState()
{
    state->Release();
}

void SamplerState::SetState(UINT slot)
{
    // PixelShader�� SamplerState ����
    DC->PSSetSamplers(slot, 1, &state);
}

void SamplerState::Filter(D3D11_FILTER filter)
{
    desc.Filter = filter;

    Changed();
}

void SamplerState::Changed()
{
    // ������Ʈ ������ ����Ǿ� �ٽ� ����
    if (state != nullptr)
        state->Release();

    DEVICE->CreateSamplerState(&desc, &state);
}
