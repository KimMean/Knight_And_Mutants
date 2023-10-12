#include "Framework.h"
#include "InitializedScene.h"

InitializedScene::InitializedScene()
{
    vertexShader = Shader::AddVertexShader(L"Shader/VertexShader.hlsl");
    pixelShader = Shader::AddPixelShader(L"Shader/PixelShader.hlsl");
    
    // ���ؽ� ����
    vertices.emplace_back(-0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    vertices.emplace_back(0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    vertices.emplace_back(-0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    vertices.emplace_back(0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

    // �ε��� ����
    indices = {
        0, 1, 2, 2, 1, 3
    };

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), vertices.size());
    
    indexBuffer = new IndexBuffer(indices.data(), indices.size());
    
}

InitializedScene::~InitializedScene()
{
    delete vertexBuffer;
    delete indexBuffer;
}

void InitializedScene::Update()
{
}

void InitializedScene::Render()
{
    // ���ؽ�, �ε��� ���� �Է� ����
    vertexBuffer->IASet();
    indexBuffer->IASet();

    vertexShader->SetShader();
    pixelShader->SetShader();

    DC->DrawIndexed(indices.size(), 0, 0);         // ������ VS, PS�� ����Ͽ� �׸���
}

void InitializedScene::Test()
{
    //HRESULT result;
    //UINT width = WIN_WIDTH;
    //UINT height = WIN_HEIGHT;
    //
    //// DXGI ����̽� �� ����ü�� ����
    //// Swap Chain ����
    //DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    //// ȭ�� ũ��
    //swapChainDesc.BufferDesc.Width = width;
    //swapChainDesc.BufferDesc.Height = height;
    //// ���� ����
    //swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //// �ֻ���
    //swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    //swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    //// ��Ƽ ���ø�(��Ƽ�ٸ���� ����)
    //swapChainDesc.SampleDesc.Count = 1;
    //swapChainDesc.SampleDesc.Quality = 0;
    //// ���� �뵵 ����
    //swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    //swapChainDesc.BufferCount = 1;
    //swapChainDesc.OutputWindow = hWnd;
    //// ��üȭ�� X
    //swapChainDesc.Windowed = TRUE;
    //// �ĸ� ���۸� ��� ó�������� ����
    //swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    //
    //UINT flags = D3D11_CREATE_DEVICE_DEBUG;
    //
    //result = D3D11CreateDeviceAndSwapChain(
    //    nullptr,                    // �� �Ű������� nullptr�� �����ϸ� �⺻ �׷��� ī�带 ����մϴ�.
    //    D3D_DRIVER_TYPE_HARDWARE,   // �ϵ���� ����̹��� ����ϵ��� ����
    //    nullptr,                    // ����Ʈ���� ����� �ڵ��� �����մϴ�. �� �Ű������� nullptr�� �����ϸ� ����Ʈ���� �������� ������� �ʽ��ϴ�.
    //    flags,                      // D3D11_CREATE_DEVICE_DEBUG ���
    //    nullptr,                    // nullptr�� �����ϸ� �ִ� ������ ���õ˴ϴ�.
    //    0,                          // ��� ������ ���� �����մϴ�. 
    //    D3D11_SDK_VERSION,          // DirectX 11�� ����ϵ��� �����մϴ�.
    //    &swapChainDesc,             // ���� ü���� �Ӽ��� �����ϴ� ����ü�� �����մϴ�.
    //    &swapChain,                 // ������ ���� ü���� �����͸� ��ȯ�մϴ�.
    //    &device,                    // ������ ����̽��� �����͸� ��ȯ�մϴ�.
    //    nullptr,                    // ������ ����̽��� ��� ������ ��ȯ�մϴ�.
    //    &context                    // ������ ����̽��� ���ؽ�Ʈ�� ��ȯ�մϴ�.
    //);
    //
    //if (FAILED(result)) { return; }
    //
    //// ����� �� ���� ���ٽ� ���� ����
    //ID3D11Texture2D* backBuffer;
    ////ID3D11Texture2D* depthBuffer;
    //
    //// ���� ü���� �ĸ� ���۸� �����ɴϴ�.
    //swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    //// �ĸ� ���ۿ� ���� ���� Ÿ�� ��(ID3D11RenderTargetView)�� �����մϴ�.
    //device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    //backBuffer->Release();
    //
    //// ���� Ÿ�� �並 ��� ����(OM) �ܰ��� ���� Ÿ������ �����մϴ�. 
    //context->OMSetRenderTargets(1, &renderTargetView, nullptr);

    // ����Ʈ ���� �� ����
    //D3D11_VIEWPORT viewport;
    //// ����Ʈ ������ �ʺ�� �����Դϴ�.
    //viewport.Width = width;
    //viewport.Height = height;
    //// ����Ʈ ������ ���� �� ������ �����մϴ�.
    //viewport.MinDepth = 0.0f;
    //viewport.MaxDepth = 1.0f;
    //// ����Ʈ ������ ���� �� �𼭸��� ��ǥ�Դϴ�.
    //viewport.TopLeftX = 0;
    //viewport.TopLeftY = 0;
    //
    //// �����Ͷ������� �� ��Ʈ ����
    //DC->RSSetViewports(1, &viewport);

    // ���̴� �� �Է� ���̾ƿ� ����
    // ���� ���̴� ������
    //vertexShader = Shader::AddVertexShader(L"Shader/VertexShader.hlsl");
    //pixelShader = Shader::AddPixelShader(L"Shader/PixelShader.hlsl");
    //ID3DBlob* vertexShaderBlob;
    //result = D3DCompileFromFile(
    //    L"Shader/VertexShader.hlsl",    // ��� ���� ���ڿ� ������
    //    nullptr,                        // ��ó���� ��ũ�� ����
    //    nullptr,                        // �����ϴ� �ٸ� ������ ���
    //    "main",                         // ������ �Լ� �̸�
    //    "vs_5_0",                       // ���̴� �ڵ��� ��� ����
    //    D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,    // ������ �ɼ�
    //    0,                              // ����Ǿ� ���� �׻� 0?
    //    &vertexShaderBlob,              // �����ϵ� ���̴� �ڵ尡 ����� ��ġ
    //    nullptr                         // ������ ���� �޽��� ���� ��ġ
    //);
    //
    //if (FAILED(result)) { return; }
    //
    //// ���� ���̴� ����
    //result = DEVICE->CreateVertexShader(
    //    vertexShaderBlob->GetBufferPointer(),   // HLSL �ڵ忡 ���� �������Դϴ�.
    //    vertexShaderBlob->GetBufferSize(),      // HLSL �ڵ��� �����Դϴ�.
    //    nullptr,                                // ID3D11ClassLinkage �������̽��� ���� ������
    //    &vertexShader                           // ID3D11VertexShader �������̽� ��ü�� ���� �������Դϴ�.
    //);
    //
    //if (FAILED(result)) { return; }



    // �ȼ� ���̴� ������
    //ID3DBlob* pixelShaderBlob;
    //result = D3DCompileFromFile(
    //    L"Shader/PixelShader.hlsl",
    //    nullptr,
    //    nullptr,
    //    "main",
    //    "ps_5_0",
    //    D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,
    //    0,
    //    &pixelShaderBlob,
    //    nullptr
    //);
    //
    //if (FAILED(result)) { return; }
    //
    //// �ȼ� ���̴� ����
    //result = DEVICE->CreatePixelShader(
    //    pixelShaderBlob->GetBufferPointer(),
    //    pixelShaderBlob->GetBufferSize(),
    //    nullptr,
    //    &pixelShader
    //);
    //
    //if (FAILED(result)) { return; }

    // ��ǲ ���̾ƿ� ����
    //D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[2];
    //inputLayoutDesc[0].SemanticName = "POSITION";
    //inputLayoutDesc[0].SemanticIndex = 0;
    //inputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    //inputLayoutDesc[0].InputSlot = 0;
    //inputLayoutDesc[0].AlignedByteOffset = 0;
    //inputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    //inputLayoutDesc[0].InstanceDataStepRate = 0;
    //
    //inputLayoutDesc[1].SemanticName = "COLOR";
    //inputLayoutDesc[1].SemanticIndex = 0;
    //inputLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    //inputLayoutDesc[1].InputSlot = 0;
    //inputLayoutDesc[1].AlignedByteOffset = 12;
    //inputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    //inputLayoutDesc[1].InstanceDataStepRate = 0;
    //
    //UINT layoutSize = ARRAYSIZE(inputLayoutDesc);
    //// ��ǲ ���̾ƿ� ����
    //result = DEVICE->CreateInputLayout(
    //    inputLayoutDesc,        // D3D11_INPUT_ELEMENT_DESC ����ü �迭�� ������
    //    layoutSize,             // �迭�� ����
    //    vertexShaderBlob->GetBufferPointer(),   // ���̴� �ڵ��� ����Ʈ�ڵ��Դϴ�. 
    //    vertexShaderBlob->GetBufferSize(),      // ����Ʈ ��
    //    &inputLayout            // �Է� ���̾ƿ��� ������
    //);
    //
    //
    //if (FAILED(result)) { return; }

    // ���ؽ� ����
       //vertices.emplace_back(-0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
       //vertices.emplace_back(0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
       //vertices.emplace_back(-0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
       //vertices.emplace_back(0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
       //
       //// �ε��� ����
       //indices = {
       //    0, 1, 2, 2, 1, 3
       //};

    //stride = sizeof(Vertex);    // ����ü ũ��
    //offset = 0;
    //
    // ���ؽ� ���� ����

    //vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), vertices.size());
    //D3D11_BUFFER_DESC vertexBufferDesc = {};
    //vertexBufferDesc.ByteWidth = stride * vertices.size(); // ���ؽ� 3��
    //vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;   // ���� ����� ����
    //vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // � ���۷� �������
    //vertexBufferDesc.CPUAccessFlags = 0;    // D3D11_CPU_ACCESS_WRITE
    //vertexBufferDesc.MiscFlags = 0;         // ���ҽ��� �ٸ� �Ӽ��� �����ϴ� �� ���˴ϴ�.
    //
    //// ���ؽ� ������ ����
    //D3D11_SUBRESOURCE_DATA vertexData = {};
    //vertexData.pSysMem = vertices.data();   // �ʱ� �����Ͱ� �ִ� �޸��� ������
    //vertexData.SysMemPitch = 0;             // �� ������ ����Ʈ ������ ����
    //vertexData.SysMemSlicePitch = 0;        // ���긮�ҽ��� ���� �����̽� ������ ����Ʈ ������ ����
    //
    //// ���ؽ� ���� ����
    //DEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

    //indexBuffer = new IndexBuffer(indices.data(), indices.size());
    //// �ε��� ���� ����
    //D3D11_BUFFER_DESC indexBufferDesc = {};
    //indexBufferDesc.ByteWidth = sizeof(UINT) * indices.size(); // �ε��� 3��
    //indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    //indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    //indexBufferDesc.CPUAccessFlags = 0;
    //indexBufferDesc.MiscFlags = 0;
    //
    //// �ε��� ������ ����
    //D3D11_SUBRESOURCE_DATA indexData = {};
    //indexData.pSysMem = indices.data();
    //indexData.SysMemPitch = 0;
    //indexData.SysMemSlicePitch = 0;
    //
    //// �ε��� ���� ����
    //DEVICE->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

    // ȭ�� Ŭ����
    //float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    //DC->ClearRenderTargetView(renderTargetView, clearColor);
    // 
    // ���ؽ�, �ε��� ���� �Է� ����
    //vertexBuffer->IASet();
    //indexBuffer->IASet();
    ////DC->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    ////DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ////DC->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    //
    //vertexShader->SetShader();
    ////DC->IASetInputLayout(inputLayout);             // VertexShader ���� ����
    ////DC->VSSetShader(vertexShader, nullptr, 0);     // VertexShader ����
    //pixelShader->SetShader();
    ////DC->PSSetShader(pixelShader, nullptr, 0);      // PixelShader ����
    //
    //DC->DrawIndexed(indices.size(), 0, 0);         // ������ VS, PS�� ����Ͽ� �׸���
    // 
    //Device::Get()->Present();
    //swapChain->Present(0, 0);   // �� ����(Back Buffer)�� �׷��� ȭ���� ����Ʈ ����(Front Buffer)�� ����
}

void InitializedScene::PreRender()
{
}

void InitializedScene::PostRender()
{
}

void InitializedScene::GUIRender()
{
}
