#include "Framework.h"

RasterizerState::RasterizerState()
{
    desc.FillMode = D3D11_FILL_SOLID;   // solid 폴리곤을 채우는 방법을 지정
    desc.CullMode = D3D11_CULL_BACK;    // back 폴리곤의 어느 면을 제거할지 지정
    //desc.FrontCounterClockwise;       // false 폴리곤의 앞면을 결정할 때 사용할 방향을 지정합니다. 기본값은 시계방향(Clockwise)입니다.
    //desc.DepthBias;       // 0 깊이 값 계산에서 사용할 Bias 값을 지정합니다. 이 값은 Z-Fighting 문제를 해결하기 위해 사용됩니다.
    //desc.DepthBiasClamp;      // 0.0f z 바이어스의 크기에 대한 상한값(양수 또는 음수)을 제공
    //desc.SlopeScaledDepthBias;    // 0.0f 주어진 픽셀의 기울기에 대한 스칼라
    //desc.DepthClipEnable;     // true 이 값을 사용하면 화면에서 벗어나는 폴리곤의 렌더링을 방지할 수 있습니다.
    //desc.ScissorEnable;       // false Scissor 테스트는 특정 영역 내의 픽셀만 렌더링하도록 제한하는 기능입니다.
    //desc.MultisampleEnable;   // false 다중 샘플 앤티앨리어싱(MSAA) 렌더 대상에서 사변형 또는 알파 라인 앤티앨리어싱 알고리즘을 사용할지 여부를 지정
    //desc.AntialiasedLineEnable;   // false 선 앤티앨리어싱을 활성화할지 여부를 지정

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

// 스카이 박스나 안쪽에 있을 때
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
