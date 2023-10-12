#include "Framework.h"

Shadow::Shadow(UINT width, UINT height)
    : width(width), height(height)
{
    renderTarget = new RenderTarget(width, height);
    depthStencil = new DepthStencil(width, height);

    viewBuffer = new ViewBuffer();
    projectionBuffer = new MatrixBuffer();

    // ������ ����
    quad = new Quad(Vector2(200, 200));
    quad->gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
    quad->gameObject->transform->Pos() = { 100, 200, 0.0f };
    quad->gameObject->transform->UpdateWorld();
    texture = Texture::AddTexture(L"DepthMap", renderTarget->GetSRV());
    quad->gameObject->material->SetDiffuseMap(texture);

    transform = new Transform();
    transform->SetTag("ShadowPos");
    transform->Load();
    DirectLight* light = Environment::Get()->GetDirectLight();
    light->GetLight().pos = transform->Pos();
    //light->GetLight().direction = transform->Rot().GetNormalized() * -1.0f;
    light->GetLight().range = 2000.0f;
}

Shadow::~Shadow()
{
    delete renderTarget;
    delete depthStencil;

    delete viewBuffer;
    delete projectionBuffer;

    delete quad;
}

// ���� Ÿ�� ����
void Shadow::SetRenderTarget()
{
    renderTarget->Set(depthStencil);
    SetViewProjection();
}

// ������ ����
void Shadow::SetRender()
{
    viewBuffer->SetVertexShaderBuffer(11);
    projectionBuffer->SetVertexShaderBuffer(12);

    DC->PSSetShaderResources(9, 1, &renderTarget->GetSRV());
}

void Shadow::PostRender()
{
    quad->Render();
}

void Shadow::GUIRender()
{
    ImGui::SliderFloat("Scale", &scale, 1.0f, 10.0f);
    transform->GUIRender();
}

// ���� �������� �� ���ۿ� �������� ���۸� ����� ����
void Shadow::SetViewProjection()
{
    DirectLight* light = Environment::Get()->GetDirectLight();
    // ���� �������� ��
    //Matrix view = XMMatrixLookAtLH(Vector3(light->GetLight().pos),
    //    Vector3(light->GetLight().direction), Vector3(0, 1, 0));
    transform->UpdateWorld();
    Matrix view = XMMatrixInverse(nullptr, transform->GetWorld());
    viewBuffer->SetMatrix(view, transform->GetWorld());
    // ���� �������� ��������
    //Matrix projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.0f, 0.1f, 2000.0f);
    Matrix projection = XMMatrixPerspectiveFovLH(
        XM_PIDIV4,      // ���� �þ߰�
        (float)WIN_WIDTH / (float)WIN_HEIGHT,   // ��Ⱦ��
        0.1f,           // Near Plane
        1000.0f);       // Far Plane
    //viewBuffer->SetMatrix(view, XMMatrixInverse(nullptr, view));
    projectionBuffer->SetMatrix(projection);

    viewBuffer->SetVertexShaderBuffer(1);
    projectionBuffer->SetVertexShaderBuffer(2);
}
