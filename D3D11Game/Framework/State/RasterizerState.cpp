#include "Framework.h"

RasterizerState::RasterizerState()
{
    desc.FillMode = D3D11_FILL_SOLID;   // solid �������� ä��� ����� ����
    desc.CullMode = D3D11_CULL_BACK;    // back �������� ��� ���� �������� ����
    //desc.FrontCounterClockwise;       // false �������� �ո��� ������ �� ����� ������ �����մϴ�. �⺻���� �ð����(Clockwise)�Դϴ�.
    //desc.DepthBias;       // 0 ���� �� ��꿡�� ����� Bias ���� �����մϴ�. �� ���� Z-Fighting ������ �ذ��ϱ� ���� ���˴ϴ�.
    //desc.DepthBiasClamp;      // 0.0f z ���̾�� ũ�⿡ ���� ���Ѱ�(��� �Ǵ� ����)�� ����
    //desc.SlopeScaledDepthBias;    // 0.0f �־��� �ȼ��� ���⿡ ���� ��Į��
    //desc.DepthClipEnable;     // true �� ���� ����ϸ� ȭ�鿡�� ����� �������� �������� ������ �� �ֽ��ϴ�.
    //desc.ScissorEnable;       // false Scissor �׽�Ʈ�� Ư�� ���� ���� �ȼ��� �������ϵ��� �����ϴ� ����Դϴ�.
    //desc.MultisampleEnable;   // false ���� ���� ��Ƽ�ٸ����(MSAA) ���� ��󿡼� �纯�� �Ǵ� ���� ���� ��Ƽ�ٸ���� �˰����� ������� ���θ� ����
    //desc.AntialiasedLineEnable;   // false �� ��Ƽ�ٸ������ Ȱ��ȭ���� ���θ� ����

    Changed();
}

RasterizerState::~RasterizerState()
{
    rasterizerState->Release();
}

void RasterizerState::SetRasterizerState()
{
    DC->RSSetState(rasterizerState);
}

void RasterizerState::SetCullMode(D3D11_CULL_MODE mode)
{
    desc.CullMode = mode;
    Changed();
}

void RasterizerState::SetFillMode(D3D11_FILL_MODE value)
{
    desc.FillMode = value;
    Changed();
}

// ��ī�� �ڽ��� ���ʿ� ���� ��
void RasterizerState::SetFrontCounterClockwise(bool value)
{
    desc.FrontCounterClockwise = value;

    Changed();
}

void RasterizerState::Changed()
{
    if (rasterizerState != nullptr)
        rasterizerState->Release();

    DEVICE->CreateRasterizerState(&desc, &rasterizerState);
}
