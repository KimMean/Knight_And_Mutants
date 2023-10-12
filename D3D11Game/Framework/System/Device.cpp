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
    // ����Ʈ ���� �� ����
    D3D11_VIEWPORT viewport;
    // ����Ʈ ������ �ʺ�� �����Դϴ�.
    viewport.Width = WIN_WIDTH;
    viewport.Height = WIN_HEIGHT;
    // ����Ʈ ������ ���� �� ������ �����մϴ�.
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    // ����Ʈ ������ ���� �� �𼭸��� ��ǥ�Դϴ�.
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    // �����Ͷ������� �� ��Ʈ ����
    deviceContext->RSSetViewports(1, &viewport);
}

void Device::Clear()
{
    // ���� Ÿ�� �並 ��� ����(OM) �ܰ��� ���� Ÿ������ �����մϴ�. 
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
    // Swap Chain ����
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    // ȭ�� ũ��
    swapChainDesc.BufferDesc.Width = WIN_WIDTH;
    swapChainDesc.BufferDesc.Height = WIN_HEIGHT;
    // ���� ����
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    // �ֻ���
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    // ��Ƽ ���ø�(��Ƽ�ٸ���� ����)
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    // ���� �뵵 ����
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hWnd;
    // ��üȭ�� X
    swapChainDesc.Windowed = TRUE;
    // �ĸ� ���۸� ��� ó�������� ����
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Font ������ flag (D3D11_CREATE_DEVICE_BGRA_SUPPORT)
    UINT flags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,                    // �� �Ű������� nullptr�� �����ϸ� �⺻ �׷��� ī�带 ����մϴ�.
        D3D_DRIVER_TYPE_HARDWARE,   // �ϵ���� ����̹��� ����ϵ��� ����
        nullptr,                    // ����Ʈ���� ����� �ڵ��� �����մϴ�. �� �Ű������� nullptr�� �����ϸ� ����Ʈ���� �������� ������� �ʽ��ϴ�.
        flags,                      // D3D11_CREATE_DEVICE_DEBUG ���
        nullptr,                    // nullptr�� �����ϸ� �ִ� ������ ���õ˴ϴ�.
        0,                          // ��� ������ ���� �����մϴ�. 
        D3D11_SDK_VERSION,          // DirectX 11�� ����ϵ��� �����մϴ�.
        &swapChainDesc,             // ���� ü���� �Ӽ��� �����ϴ� ����ü�� �����մϴ�.
        &swapChain,                 // ������ ���� ü���� �����͸� ��ȯ�մϴ�.
        &device,                    // ������ ����̽��� �����͸� ��ȯ�մϴ�.
        nullptr,                    // ������ ����̽��� ��� ������ ��ȯ�մϴ�.
        &deviceContext              // ������ ����̽��� ���ؽ�Ʈ�� ��ȯ�մϴ�.
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED CreateDeviceAndSwapChain()");
    }
}

void Device::CreateBackBufferAndDepthStencillBuffer()
{
    // ����� ����
    ID3D11Texture2D* backBuffer;
    //ID3D11Texture2D* depthBuffer;

    // ���� ü���� �ĸ� ���۸� �����ɴϴ�.
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    // �ĸ� ���ۿ� ���� ���� Ÿ�� ��(ID3D11RenderTargetView)�� �����մϴ�.
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();

    // ���� ���� ����
    ID3D11Texture2D* depthBuffer;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = WIN_WIDTH;    // �ؽ�ó�� ���α���
    depthDesc.Height = WIN_HEIGHT;  // �ؽ�ó�� ���α���
    depthDesc.MipLevels = 1;        // �ؽ�ó �ػ��� ��ȭ�� ���� �����Ǵ� �߰�����
    depthDesc.ArraySize = 1;        // �ؽ�ó �迭�� ���Ե� �ؽ�ó ��
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;   // �ȼ� ����
    depthDesc.SampleDesc.Count = 1;     
    depthDesc.Usage = D3D11_USAGE_DEFAULT;  // �ؽ�ó ��� ���
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // �ؽ�ó�� ��� ���ε��Ұ���

    device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
    depthViewDesc.Format = depthDesc.Format;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    // �������ٽǺ並 ����
    device->CreateDepthStencilView(depthBuffer, &depthViewDesc, &depthStencilView);

}
