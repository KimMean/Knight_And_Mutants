#include "Framework.h"

Reflection::Reflection(Transform* target)
    : target(target)
{
    renderTarget = new RenderTarget(2048, 2048);
    depthStencil = new DepthStencil(2048, 2048);

    camera = new Camera();

    quad = new Quad(Vector2(500, 500));
    quad->gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
    quad->gameObject->transform->Pos() = { 300, 300, 0 };
    Texture* texture = Texture::AddTexture(L"Reflection", renderTarget->GetSRV());
    quad->gameObject->material->SetDiffuseMap(texture);
    //quad->gameObject->material->SetDiffuseMap(L"Textures/Landscape/Dirt.png");
    quad->gameObject->transform->UpdateWorld();
}

Reflection::~Reflection()
{
    delete renderTarget;
    delete depthStencil;
    delete camera;
    delete quad;
}

void Reflection::Update()
{
    camera->transform->Pos() = CAM->transform->Pos();
    camera->transform->Rot() = CAM->transform->Rot();
    // 보고 있는 방향에서 x축으로 반전시켜서 바라보면 반사된 각도가 나옴
    camera->transform->Rot().x *= -1.0f;
    camera->transform->Pos().y = target->Pos().y * 2.0f - camera->transform->Pos().y;

    camera->transform->UpdateWorld();
}

void Reflection::SetReflection()
{
    renderTarget->Set(depthStencil);
    camera->SetView();
}

void Reflection::SetRender()
{
    camera->GetViewBuffer()->SetVertexShaderBuffer(10);
    DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());
}

void Reflection::PostRender()
{
    quad->Render();
}
