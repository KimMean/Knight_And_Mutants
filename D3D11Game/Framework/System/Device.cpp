#include "Framework.h"

Device::Device()
{
    CreateDeviceAndSwapChain();
    CreateBackBufferAndDepthStencillBuffer();
    //SetViewport();
}

Device::~Device()
{
    device->Release();
    deviceContext->Release();

    swapChain->Release();
    renderTargetView->Release();
    depthStencilView->Release();
}

void Device::SetViewport()
{
    // 뷰포트 생성 및 설정
    D3D11_VIEWPORT viewport;
    // 뷰포트 영역의 너비와 높이입니다.
    viewport.Width = WIN_WIDTH;
    viewport.Height = WIN_HEIGHT;
    // 뷰포트 영역의 깊이 값 범위를 지정합니다.
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    // 뷰포트 영역의 왼쪽 위 모서리의 좌표입니다.
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    // 래스터라이저의 뷰 포트 설정
    deviceContext->RSSetViewports(1, &viewport);
}

void Device::Clear()
{
    // 렌더 타겟 뷰를 출력 병합(OM) 단계의 렌더 타겟으로 설정합니다. 
    //deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::Present()
{
    swapChain->Present(0, 0);
}

void Device::CreateDeviceAndSwapChain()
{
    // Swap Chain 설정
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    // 화면 크기
    swapChainDesc.BufferDesc.Width = WIN_WIDTH;
    swapChainDesc.BufferDesc.Height = WIN_HEIGHT;
    // 색상 포맷
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    // 주사율
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    // 멀티 샘플링(안티앨리어싱 설정)
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    // 버퍼 용도 지정
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hWnd;
    // 전체화면 X
    swapChainDesc.Windowed = TRUE;
    // 후면 버퍼를 어떻게 처리할지를 결정
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Font 지원용 flag (D3D11_CREATE_DEVICE_BGRA_SUPPORT)
    UINT flags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,                    // 이 매개변수를 nullptr로 지정하면 기본 그래픽 카드를 사용합니다.
        D3D_DRIVER_TYPE_HARDWARE,   // 하드웨어 드라이버를 사용하도록 지정
        nullptr,                    // 소프트웨어 모듈의 핸들을 지정합니다. 이 매개변수를 nullptr로 지정하면 소프트웨어 렌더러를 사용하지 않습니다.
        flags,                      // D3D11_CREATE_DEVICE_DEBUG 사용
        nullptr,                    // nullptr로 지정하면 최대 레벨이 선택됩니다.
        0,                          // 기능 레벨의 수를 지정합니다. 
        D3D11_SDK_VERSION,          // DirectX 11을 사용하도록 지정합니다.
        &swapChainDesc,             // 스왑 체인의 속성을 지정하는 구조체를 지정합니다.
        &swapChain,                 // 생성된 스왑 체인의 포인터를 반환합니다.
        &device,                    // 생성된 디바이스의 포인터를 반환합니다.
        nullptr,                    // 생성된 디바이스의 기능 레벨을 반환합니다.
        &deviceContext              // 생성된 디바이스의 컨텍스트를 반환합니다.
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED CreateDeviceAndSwapChain()");
    }
}

void Device::CreateBackBufferAndDepthStencillBuffer()
{
    // 백버퍼 생성
    ID3D11Texture2D* backBuffer;
    //ID3D11Texture2D* depthBuffer;

    // 스왑 체인의 후면 버퍼를 가져옵니다.
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    // 후면 버퍼에 대한 렌더 타겟 뷰(ID3D11RenderTargetView)를 생성합니다.
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();

    // 뎁스 버퍼 생성
    ID3D11Texture2D* depthBuffer;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = WIN_WIDTH;    // 텍스처의 가로길이
    depthDesc.Height = WIN_HEIGHT;  // 텍스처의 세로길이
    depthDesc.MipLevels = 1;        // 텍스처 해상도의 변화에 따라 생성되는 추가레벨
    depthDesc.ArraySize = 1;        // 텍스처 배열에 포함된 텍스처 수
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;   // 픽셀 형식
    depthDesc.SampleDesc.Count = 1;     
    depthDesc.Usage = D3D11_USAGE_DEFAULT;  // 텍스처 사용 방법
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // 텍스처를 어디에 바인딩할건지

    device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
    depthViewDesc.Format = depthDesc.Format;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    // 뎁스스텐실뷰를 생성
    device->CreateDepthStencilView(depthBuffer, &depthViewDesc, &depthStencilView);

}
