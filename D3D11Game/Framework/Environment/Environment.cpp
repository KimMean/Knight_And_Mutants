#include "Framework.h"

Environment::Environment()
{
    CreateProjection(); // projection��� ����
    CreateState();      // �������� ���� ����

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
    DC->RSSetViewports(1, &viewport);
}

void Environment::SetPerspective()
{
    // Matrix projection : register(b2) 2���� ����
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
    // SetPixelShaderBuffer 0���� Light ����
    directLight->SetPixelShaderBuffer(0);
}

void Environment::CreateProjection()
{
    // ��� ���� ���(�������� ���)�� ����
    orthographic = XMMatrixOrthographicOffCenterLH(
        0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);

    // ���� ���� ���(�������� ���)�� ����
    perspective = XMMatrixPerspectiveFovLH(
        XM_PIDIV4,      // ���� �þ߰�
        (float)WIN_WIDTH / (float)WIN_HEIGHT,   // ��Ⱦ��
        0.1f,           // Near Plane
        1000.0f);       // Far Plane

    projectionBuffer = new MatrixBuffer();
}

void Environment::CreateState()
{
    // �ؽ��� ���ø� ��� ����
    samplerState = new SamplerState();

    // �������� �׸��� ��� ����
    rasterizerState = new RasterizerState();

    // ���� ��� ����
    blendState = new BlendState();

    // �������ٽ� ���� ��� ����
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
