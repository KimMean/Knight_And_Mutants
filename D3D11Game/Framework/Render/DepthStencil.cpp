#include "Framework.h"
#include "DepthStencil.h"
DepthStencil::DepthStencil(UINT width, UINT height, bool isStencil)
    : width(width), height(height), isStencil(isStencil)
{
    CreateDSVTexture();
    CreateDSV();
    CreateSRV();
}

DepthStencil::~DepthStencil()
{
    dsvTexture->Release();
    dsv->Release();
    srv->Release();
}

void DepthStencil::Clear()
{
    DC->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

// 2D 텍스처(이미지 또는 데이터를 저장하는 버퍼)를 생성
void DepthStencil::CreateDSVTexture()
{
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    // 24 비트의 깊이 정보와 8 비트의 스텐실 정보를 포함하는 형식과
    // 32 비트의 깊이 정보를 포함하는 형식
    depthDesc.Format = isStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    DEVICE->CreateTexture2D(&depthDesc, nullptr, &dsvTexture);
}

// DepthStencilView 생성
void DepthStencil::CreateDSV()
{
    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
    depthViewDesc.Format = isStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    DEVICE->CreateDepthStencilView(dsvTexture, &depthViewDesc, &dsv);
}

// ShaderResourceView 생성
void DepthStencil::CreateSRV()
{
    D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
    desc.Format = isStencil ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_FLOAT;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipLevels = 1;

    DEVICE->CreateShaderResourceView(dsvTexture, &desc, &srv);
}
