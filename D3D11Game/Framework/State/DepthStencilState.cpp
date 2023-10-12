#include "Framework.h"

DepthStencilState::DepthStencilState()
{
    desc.DepthEnable = true;    // 깊이 버퍼를 사용할지 여부를 나타내는 값
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이 버퍼에 쓰는 방법을 지정하는 값
    desc.DepthFunc = D3D11_COMPARISON_LESS; // 깊이 테스트를 수행할 때 사용되는 비교 함수
    desc.StencilEnable = true;      // 스텐실 기능을 사용할지 여부를 나타내는 값
    desc.StencilReadMask = 0xff;    // 스텐실 버퍼의 읽기 마스크를 나타내는 값
    desc.StencilWriteMask = 0xff;   // 스텐실 버퍼의 쓰기 마스크를 나타내는 값
    //desc.FrontFace;   // 정면 면의 스텐실 오퍼레이션을 설정하는 D3D11_DEPTH_STENCILOP_DESC 구조체입니다.
    //desc.BackFace;    // 뒷면 면의 스텐실 오퍼레이션을 설정하는 D3D11_DEPTH_STENCILOP_DESC 구조체입니다.

    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;   // 스텐실 테스트가 실패한 경우 수행되는 오퍼레이션
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;  // 깊이 테스트는 통과하지만 스텐실 테스트가 실패한 경우 수행되는 오퍼레이션입니다.
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;   // 스텐실 및 깊이 테스트 모두 통과한 경우 수행되는 오퍼레이션입니다.
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;   // 스텐실 테스트에서 사용되는 비교 함수입니다.

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
