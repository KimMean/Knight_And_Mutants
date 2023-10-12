#include "Framework.h"

SamplerState::SamplerState()
{
    //텍스처를 샘플링할 때 사용할 필터링 방법을 지정
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    // 텍스처 좌표가 범위를 벗어났을 때 처리 방법을 지정합니다.
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    // 맵 레벨을 선택할 때 사용되는 바이어스 값을 지정
    //desc.MipLODBias;
    // 필터링을 사용할 때 적용되는 최대 표본화 수를 지정
    //desc.MaxAnisotropy;
    // 텍스처 값을 비교할 때 사용할 비교 함수를 지정합니다.
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    // 텍스처 좌표가 경계 값을 벗어났을 때 사용할 색상 값을 지정
    //desc.BorderColor[4];
    // 카메라의 거리에 따라서 퀄리티를 다르게 조절할 때 사용
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
    // PixelShader에 SamplerState 설정
    DC->PSSetSamplers(slot, 1, &state);
}

void SamplerState::Filter(D3D11_FILTER filter)
{
    desc.Filter = filter;

    Changed();
}

void SamplerState::Changed()
{
    // 스테이트 정보가 변경되어 다시 생성
    if (state != nullptr)
        state->Release();

    DEVICE->CreateSamplerState(&desc, &state);
}
