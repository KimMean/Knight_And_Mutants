#include "Framework.h"
#include "InitializedScene.h"

InitializedScene::InitializedScene()
{
    vertexShader = Shader::AddVertexShader(L"Shader/VertexShader.hlsl");
    pixelShader = Shader::AddPixelShader(L"Shader/PixelShader.hlsl");
    
    // 버텍스 정보
    vertices.emplace_back(-0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    vertices.emplace_back(0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    vertices.emplace_back(-0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    vertices.emplace_back(0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

    // 인덱스 정보
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
    // 버텍스, 인덱스 정보 입력 조립
    vertexBuffer->IASet();
    indexBuffer->IASet();

    vertexShader->SetShader();
    pixelShader->SetShader();

    DC->DrawIndexed(indices.size(), 0, 0);         // 설정된 VS, PS를 사용하여 그리기
}

void InitializedScene::Test()
{
    //HRESULT result;
    //UINT width = WIN_WIDTH;
    //UINT height = WIN_HEIGHT;
    //
    //// DXGI 디바이스 및 스왑체인 생성
    //// Swap Chain 설정
    //DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    //// 화면 크기
    //swapChainDesc.BufferDesc.Width = width;
    //swapChainDesc.BufferDesc.Height = height;
    //// 색상 포맷
    //swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //// 주사율
    //swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    //swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    //// 멀티 샘플링(안티앨리어싱 설정)
    //swapChainDesc.SampleDesc.Count = 1;
    //swapChainDesc.SampleDesc.Quality = 0;
    //// 버퍼 용도 지정
    //swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    //swapChainDesc.BufferCount = 1;
    //swapChainDesc.OutputWindow = hWnd;
    //// 전체화면 X
    //swapChainDesc.Windowed = TRUE;
    //// 후면 버퍼를 어떻게 처리할지를 결정
    //swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    //
    //UINT flags = D3D11_CREATE_DEVICE_DEBUG;
    //
    //result = D3D11CreateDeviceAndSwapChain(
    //    nullptr,                    // 이 매개변수를 nullptr로 지정하면 기본 그래픽 카드를 사용합니다.
    //    D3D_DRIVER_TYPE_HARDWARE,   // 하드웨어 드라이버를 사용하도록 지정
    //    nullptr,                    // 소프트웨어 모듈의 핸들을 지정합니다. 이 매개변수를 nullptr로 지정하면 소프트웨어 렌더러를 사용하지 않습니다.
    //    flags,                      // D3D11_CREATE_DEVICE_DEBUG 사용
    //    nullptr,                    // nullptr로 지정하면 최대 레벨이 선택됩니다.
    //    0,                          // 기능 레벨의 수를 지정합니다. 
    //    D3D11_SDK_VERSION,          // DirectX 11을 사용하도록 지정합니다.
    //    &swapChainDesc,             // 스왑 체인의 속성을 지정하는 구조체를 지정합니다.
    //    &swapChain,                 // 생성된 스왑 체인의 포인터를 반환합니다.
    //    &device,                    // 생성된 디바이스의 포인터를 반환합니다.
    //    nullptr,                    // 생성된 디바이스의 기능 레벨을 반환합니다.
    //    &context                    // 생성된 디바이스의 컨텍스트를 반환합니다.
    //);
    //
    //if (FAILED(result)) { return; }
    //
    //// 백버퍼 및 뎁스 스텐실 버퍼 생성
    //ID3D11Texture2D* backBuffer;
    ////ID3D11Texture2D* depthBuffer;
    //
    //// 스왑 체인의 후면 버퍼를 가져옵니다.
    //swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    //// 후면 버퍼에 대한 렌더 타겟 뷰(ID3D11RenderTargetView)를 생성합니다.
    //device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    //backBuffer->Release();
    //
    //// 렌더 타겟 뷰를 출력 병합(OM) 단계의 렌더 타겟으로 설정합니다. 
    //context->OMSetRenderTargets(1, &renderTargetView, nullptr);

    // 뷰포트 생성 및 설정
    //D3D11_VIEWPORT viewport;
    //// 뷰포트 영역의 너비와 높이입니다.
    //viewport.Width = width;
    //viewport.Height = height;
    //// 뷰포트 영역의 깊이 값 범위를 지정합니다.
    //viewport.MinDepth = 0.0f;
    //viewport.MaxDepth = 1.0f;
    //// 뷰포트 영역의 왼쪽 위 모서리의 좌표입니다.
    //viewport.TopLeftX = 0;
    //viewport.TopLeftY = 0;
    //
    //// 래스터라이저의 뷰 포트 설정
    //DC->RSSetViewports(1, &viewport);

    // 셰이더 및 입력 레이아웃 생성
    // 정점 셰이더 컴파일
    //vertexShader = Shader::AddVertexShader(L"Shader/VertexShader.hlsl");
    //pixelShader = Shader::AddPixelShader(L"Shader/PixelShader.hlsl");
    //ID3DBlob* vertexShaderBlob;
    //result = D3DCompileFromFile(
    //    L"Shader/VertexShader.hlsl",    // 대상 파일 문자열 포인터
    //    nullptr,                        // 전처리기 매크로 정의
    //    nullptr,                        // 참조하는 다른 파일의 경로
    //    "main",                         // 진입점 함수 이름
    //    "vs_5_0",                       // 셰이더 코드의 대상 버전
    //    D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,    // 컴파일 옵션
    //    0,                              // 예약되어 있음 항상 0?
    //    &vertexShaderBlob,              // 컴파일된 쉐이더 코드가 저장될 위치
    //    nullptr                         // 컴파일 에러 메시지 저장 위치
    //);
    //
    //if (FAILED(result)) { return; }
    //
    //// 정점 셰이더 생성
    //result = DEVICE->CreateVertexShader(
    //    vertexShaderBlob->GetBufferPointer(),   // HLSL 코드에 대한 포인터입니다.
    //    vertexShaderBlob->GetBufferSize(),      // HLSL 코드의 길이입니다.
    //    nullptr,                                // ID3D11ClassLinkage 인터페이스에 대한 포인터
    //    &vertexShader                           // ID3D11VertexShader 인터페이스 개체에 대한 포인터입니다.
    //);
    //
    //if (FAILED(result)) { return; }



    // 픽셀 셰이더 컴파일
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
    //// 픽셀 셰이더 생성
    //result = DEVICE->CreatePixelShader(
    //    pixelShaderBlob->GetBufferPointer(),
    //    pixelShaderBlob->GetBufferSize(),
    //    nullptr,
    //    &pixelShader
    //);
    //
    //if (FAILED(result)) { return; }

    // 인풋 레이아웃 설정
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
    //// 인풋 레이아웃 생성
    //result = DEVICE->CreateInputLayout(
    //    inputLayoutDesc,        // D3D11_INPUT_ELEMENT_DESC 구조체 배열의 포인터
    //    layoutSize,             // 배열의 길이
    //    vertexShaderBlob->GetBufferPointer(),   // 쉐이더 코드의 바이트코드입니다. 
    //    vertexShaderBlob->GetBufferSize(),      // 바이트 수
    //    &inputLayout            // 입력 레이아웃의 포인터
    //);
    //
    //
    //if (FAILED(result)) { return; }

    // 버텍스 정보
       //vertices.emplace_back(-0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
       //vertices.emplace_back(0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
       //vertices.emplace_back(-0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
       //vertices.emplace_back(0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
       //
       //// 인덱스 정보
       //indices = {
       //    0, 1, 2, 2, 1, 3
       //};

    //stride = sizeof(Vertex);    // 구조체 크기
    //offset = 0;
    //
    // 버텍스 버퍼 생성

    //vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), vertices.size());
    //D3D11_BUFFER_DESC vertexBufferDesc = {};
    //vertexBufferDesc.ByteWidth = stride * vertices.size(); // 버텍스 3개
    //vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;   // 동작 방식을 지정
    //vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 어떤 버퍼로 사용할지
    //vertexBufferDesc.CPUAccessFlags = 0;    // D3D11_CPU_ACCESS_WRITE
    //vertexBufferDesc.MiscFlags = 0;         // 리소스의 다른 속성을 지정하는 데 사용됩니다.
    //
    //// 버텍스 데이터 저장
    //D3D11_SUBRESOURCE_DATA vertexData = {};
    //vertexData.pSysMem = vertices.data();   // 초기 데이터가 있는 메모리의 포인터
    //vertexData.SysMemPitch = 0;             // 행 간격을 바이트 단위로 지정
    //vertexData.SysMemSlicePitch = 0;        // 서브리소스의 깊이 슬라이스 간격을 바이트 단위로 지정
    //
    //// 버텍스 버퍼 생성
    //DEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

    //indexBuffer = new IndexBuffer(indices.data(), indices.size());
    //// 인덱스 버퍼 생성
    //D3D11_BUFFER_DESC indexBufferDesc = {};
    //indexBufferDesc.ByteWidth = sizeof(UINT) * indices.size(); // 인덱스 3개
    //indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    //indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    //indexBufferDesc.CPUAccessFlags = 0;
    //indexBufferDesc.MiscFlags = 0;
    //
    //// 인덱스 데이터 저장
    //D3D11_SUBRESOURCE_DATA indexData = {};
    //indexData.pSysMem = indices.data();
    //indexData.SysMemPitch = 0;
    //indexData.SysMemSlicePitch = 0;
    //
    //// 인덱스 버퍼 생성
    //DEVICE->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

    // 화면 클리어
    //float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    //DC->ClearRenderTargetView(renderTargetView, clearColor);
    // 
    // 버텍스, 인덱스 정보 입력 조립
    //vertexBuffer->IASet();
    //indexBuffer->IASet();
    ////DC->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    ////DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ////DC->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    //
    //vertexShader->SetShader();
    ////DC->IASetInputLayout(inputLayout);             // VertexShader 정보 적용
    ////DC->VSSetShader(vertexShader, nullptr, 0);     // VertexShader 적용
    //pixelShader->SetShader();
    ////DC->PSSetShader(pixelShader, nullptr, 0);      // PixelShader 적용
    //
    //DC->DrawIndexed(indices.size(), 0, 0);         // 설정된 VS, PS를 사용하여 그리기
    // 
    //Device::Get()->Present();
    //swapChain->Present(0, 0);   // 백 버퍼(Back Buffer)에 그려진 화면을 프론트 버퍼(Front Buffer)로 스왑
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
