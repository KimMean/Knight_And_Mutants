#include "Framework.h"

DepthStencilState::DepthStencilState()
{
    desc.DepthEnable = true;    // ���� ���۸� ������� ���θ� ��Ÿ���� ��
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // ���� ���ۿ� ���� ����� �����ϴ� ��
    desc.DepthFunc = D3D11_COMPARISON_LESS; // ���� �׽�Ʈ�� ������ �� ���Ǵ� �� �Լ�
    desc.StencilEnable = true;      // ���ٽ� ����� ������� ���θ� ��Ÿ���� ��
    desc.StencilReadMask = 0xff;    // ���ٽ� ������ �б� ����ũ�� ��Ÿ���� ��
    desc.StencilWriteMask = 0xff;   // ���ٽ� ������ ���� ����ũ�� ��Ÿ���� ��
    //desc.FrontFace;   // ���� ���� ���ٽ� ���۷��̼��� �����ϴ� D3D11_DEPTH_STENCILOP_DESC ����ü�Դϴ�.
    //desc.BackFace;    // �޸� ���� ���ٽ� ���۷��̼��� �����ϴ� D3D11_DEPTH_STENCILOP_DESC ����ü�Դϴ�.

    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;   // ���ٽ� �׽�Ʈ�� ������ ��� ����Ǵ� ���۷��̼�
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;  // ���� �׽�Ʈ�� ��������� ���ٽ� �׽�Ʈ�� ������ ��� ����Ǵ� ���۷��̼��Դϴ�.
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;   // ���ٽ� �� ���� �׽�Ʈ ��� ����� ��� ����Ǵ� ���۷��̼��Դϴ�.
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;   // ���ٽ� �׽�Ʈ���� ���Ǵ� �� �Լ��Դϴ�.

    desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    Changed();
}

DepthStencilState::~DepthStencilState()
{
    depthStencilState->Release();
}

void DepthStencilState::SetDepthStencilState()
{
    DC->OMSetDepthStencilState(depthStencilState, 1);
}

void DepthStencilState::SetDepthEnable(bool value)
{
    desc.DepthEnable = value;

    Changed();
}

void DepthStencilState::DepthWriteMask(D3D11_DEPTH_WRITE_MASK value)
{
    desc.DepthWriteMask = value;

    Changed();
}

void DepthStencilState::Changed()
{
    if (depthStencilState != nullptr)
        depthStencilState->Release();

    DEVICE->CreateDepthStencilState(&desc, &depthStencilState);
}
