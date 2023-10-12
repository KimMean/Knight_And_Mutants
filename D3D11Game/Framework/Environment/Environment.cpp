#include "Framework.h"

Environment::Environment()
{
    CreateProjection(); // projection행렬 생성
    CreateState();      // 여러가지 상태 정의

    mainCamera = new Camera();
    directLight = new DirectLight();
    uiViewBuffer = new ViewBuffer();
}

Environment::~Environment()
{
    delete projectionBuffer;
    delete directLight;
    delete uiViewBuffer;

    delete mainCamera;

    delete samplerState;
    delete rasterizerState;
    delete blendState;
    delete depthStencilState;
}

void Environment::Update()
{
    mainCamera->Update();
    DebugRenderCheck();
}

void Environment::GUIRender()
{
    if (ImGui::TreeNode("Environment"))
    {
        mainCamera->GUIRender();
        directLight->GUIRender();
        ImGui::TreePop();
    }
}

void Environment::SetState()
{
    SetViewport();
    SetPerspective();
    CAM->SetView();

    SetAlphaBlend(false);
    SetDepthEnable(true);

    samplerState->SetState();
    SetRasterizerState();
    SetBlendState();
    SetDepthStencilState();

    SetDirectLight();
}

void Environment::SetPostRender()
{
    uiViewBuffer->SetVertexShaderBuffer(1);
    SetOrthographic();

    SetAlphaBlend(true);
    SetDepthEnable(false);
    SetBlendState();
    SetDepthStencilState();
}

// RasterizerState
void Environment::SetRasterizerState()
{
    rasterizerState->SetRasterizerState();
}

void Environment::SetCullMode(D3D11_CULL_MODE mode)
{
    rasterizerState->SetCullMode(mode);
}

void Environment::SetFillMode(D3D11_FILL_MODE mode)
{
    rasterizerState->SetFillMode(mode);
    rasterizerState->SetRasterizerState();
}

void Environment::SetFrontCounterClockwise(bool value)
{
    rasterizerState->SetFrontCounterClockwise(value);
}
// BlendState
void Environment::SetBlendState()
{
    blendState->SetBlendState();
}

void Environment::SetAlphaBlend(bool value)
{
    blendState->SetAlphaBlend(value);
}

void Environment::SetAlphaToCoverage(bool value)
{
    blendState->SetAlphaToCoverage(value);
}

// DepthStencilState
void Environment::SetDepthStencilState()
{
    depthStencilState->SetDepthStencilState();
}

void Environment::SetDepthEnable(bool value)
{
    depthStencilState->SetDepthEnable(value);
}

void Environment::SetViewport(UINT width, UINT height)
{
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
    DC->RSSetViewports(1, &viewport);
}

void Environment::SetPerspective()
{
    // Matrix projection : register(b2) 2번에 설정
    projectionBuffer->SetMatrix(perspective);
    projectionBuffer->SetVertexShaderBuffer(2);
    projectionBuffer->SetPixelShaderBuffer(2);
}

void Environment::SetOrthographic()
{
    projectionBuffer->SetMatrix(orthographic);
    projectionBuffer->SetVertexShaderBuffer(2);
    projectionBuffer->SetPixelShaderBuffer(2);
}

void Environment::SetDirectLight()
{
    // SetPixelShaderBuffer 0번에 Light 설정
    directLight->SetPixelShaderBuffer(0);
}

void Environment::CreateProjection()
{
    // 평면 투영 행렬(프로젝션 행렬)을 생성
    orthographic = XMMatrixOrthographicOffCenterLH(
        0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);

    // 원근 투영 행렬(프로젝션 행렬)을 생성
    perspective = XMMatrixPerspectiveFovLH(
        XM_PIDIV4,      // 수직 시야각
        (float)WIN_WIDTH / (float)WIN_HEIGHT,   // 종횡비
        0.1f,           // Near Plane
        1000.0f);       // Far Plane

    projectionBuffer = new MatrixBuffer();
}

void Environment::CreateState()
{
    // 텍스쳐 샘플링 방법 기재
    samplerState = new SamplerState();

    // 폴리곤을 그리는 방법 기재
    rasterizerState = new RasterizerState();

    // 블렌딩 방법 기재
    blendState = new BlendState();

    // 뎁스스텐실 적용 방법 기재
    depthStencilState = new DepthStencilState();
}

void Environment::DebugRenderCheck()
{
    if (KEY_DOWN(VK_NUMPAD1))
        isCollider = !isCollider;
    if (KEY_DOWN(VK_NUMPAD2))
        isAStar = !isAStar;
    if (KEY_DOWN(VK_NUMPAD3))
        isFreeMode = !isFreeMode;
    if (KEY_DOWN(VK_NUMPAD4))
        isGUIRender = !isGUIRender;
}
