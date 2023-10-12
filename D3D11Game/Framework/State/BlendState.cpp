#include "Framework.h"

BlendState::BlendState()
{
    desc.AlphaToCoverageEnable;     // 블렌딩을 사용할지 여부를 나타내는 값입니다.
    desc.IndependentBlendEnable;    // 동시 렌더링 대상에서 독립적 혼합을 활성화할지 여부를 지정
    desc.RenderTarget[8];           

    desc.RenderTarget[0].BlendEnable = false;   // false    // 블렌딩을 사용할지 여부를 나타내는 값
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;      // D3D11_BLEND_ONE 소스 블렌딩 팩터를 지정하는 값
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;     // D3D11_BLEND_ZERO 대상 블렌딩 팩터를 지정하는 값
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;       // D3D11_BLEND_OP_ADD   블렌딩 작업을 지정하는 값
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA; // D3D11_BLEND_ONE 알파 블렌딩의 소스 블렌딩 팩터를 지정하는 값입니다.
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;// D3D11_BLEND_ZERO 알파 블렌딩의 대상 블렌딩 팩터를 지정하는 값입니다.
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;  // D3D11_BLEND_OP_ADD 알파 블렌딩 작업을 지정하는 값입니다.
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;// D3D11_COLOR_WRITE_ENABLE_ALL 렌더 타겟에 쓸 RGBA 채널을 지정하는 값입니다.
}

BlendState::~BlendState()
{
    blendState->Release();
}

void BlendState::SetBlendState()
{
    // 블렌드 상태 설정
    float blendFactor[4] = {};
    DC->OMSetBlendState(
        blendState,     // 블렌드 상태 개체
        blendFactor,    // 0.0f에서 1.0f 사이의 값으로 블렌드 팩터를 설정합니다. 이 배열은 RGBA(빨강, 녹색, 파랑, 알파) 값으로 구성
        0xffffffff      // 블렌드 마스크를 설정합니다. 이 값은 RGBA 각각에 대해 0x00000001을 의미합니다.
    );
}

void BlendState::SetAlphaBlend(bool value)
{
    desc.RenderTarget[0].BlendEnable = value;   // 블렌딩 사용 여부
    //desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    // D3D11_BLEND_INV_SRC_ALPHA 대상 픽셀의 alpha 값을 반전 시켜 원래 대상 색상에 곱합니다.
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    //desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    Changed();
}

void BlendState::SetAdditiveBlend()
{
    desc.RenderTarget[0].BlendEnable = true;
    //desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    // D3D11_BLEND_ONE 블렌딩 계산에서 대상 색상의 가중치를 나타내며, 대상 색상을 그대로 사용하는 것을 의미합니다.
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    //desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    Changed();
}
// 알파값을 사용할지 여부
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
