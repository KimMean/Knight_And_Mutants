#include "Framework.h"

BlendState::BlendState()
{
    desc.AlphaToCoverageEnable;     // ������ ������� ���θ� ��Ÿ���� ���Դϴ�.
    desc.IndependentBlendEnable;    // ���� ������ ��󿡼� ������ ȥ���� Ȱ��ȭ���� ���θ� ����
    desc.RenderTarget[8];           

    desc.RenderTarget[0].BlendEnable = false;   // false    // ������ ������� ���θ� ��Ÿ���� ��
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;      // D3D11_BLEND_ONE �ҽ� ���� ���͸� �����ϴ� ��
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;     // D3D11_BLEND_ZERO ��� ���� ���͸� �����ϴ� ��
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;       // D3D11_BLEND_OP_ADD   ���� �۾��� �����ϴ� ��
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA; // D3D11_BLEND_ONE ���� ������ �ҽ� ���� ���͸� �����ϴ� ���Դϴ�.
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;// D3D11_BLEND_ZERO ���� ������ ��� ���� ���͸� �����ϴ� ���Դϴ�.
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;  // D3D11_BLEND_OP_ADD ���� ���� �۾��� �����ϴ� ���Դϴ�.
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;// D3D11_COLOR_WRITE_ENABLE_ALL ���� Ÿ�ٿ� �� RGBA ä���� �����ϴ� ���Դϴ�.
}

BlendState::~BlendState()
{
    blendState->Release();
}

void BlendState::SetBlendState()
{
    // ���� ���� ����
    float blendFactor[4] = {};
    DC->OMSetBlendState(
        blendState,     // ���� ���� ��ü
        blendFactor,    // 0.0f���� 1.0f ������ ������ ���� ���͸� �����մϴ�. �� �迭�� RGBA(����, ���, �Ķ�, ����) ������ ����
        0xffffffff      // ���� ����ũ�� �����մϴ�. �� ���� RGBA ������ ���� 0x00000001�� �ǹ��մϴ�.
    );
}

void BlendState::SetAlphaBlend(bool value)
{
    desc.RenderTarget[0].BlendEnable = value;   // ���� ��� ����
    //desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    // D3D11_BLEND_INV_SRC_ALPHA ��� �ȼ��� alpha ���� ���� ���� ���� ��� ���� ���մϴ�.
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    //desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    Changed();
}

void BlendState::SetAdditiveBlend()
{
    desc.RenderTarget[0].BlendEnable = true;
    //desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    // D3D11_BLEND_ONE ���� ��꿡�� ��� ������ ����ġ�� ��Ÿ����, ��� ������ �״�� ����ϴ� ���� �ǹ��մϴ�.
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    //desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    Changed();
}
// ���İ��� ������� ����
void BlendState::SetAlphaToCoverage(bool value)
{
    desc.AlphaToCoverageEnable = value;

    Changed();
}

void BlendState::Changed()
{
    if (blendState != nullptr)
        blendState->Release();

    DEVICE->CreateBlendState(&desc, &blendState);
}
