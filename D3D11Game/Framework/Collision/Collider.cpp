#include "Framework.h"

bool Collider::isRender = true;

Collider::Collider()
{
    vertexShader = Shader::AddVertexShader(L"Shaders/Basic/Collider.hlsl");
    pixelShader = Shader::AddPixelShader(L"Shaders/Basic/Collider.hlsl");

    transform = new Transform();
    mesh = new Mesh<VertexColor>();
}

Collider::~Collider()
{
    delete transform;
    delete mesh;
}

void Collider::Render()
{
    //transform->UpdateWorld();
    if (!Environment::Get()->isCollider) return;
    transform->SetBuffer();

    vertexShader->SetShader();
    pixelShader->SetShader();
    
    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    //mesh->Draw();
}

bool Collider::IsCollision(Collider* collider)
{
    if (!transform->Active()) return false;
    if (!collider->transform->Active()) return false;

    switch (collider->type)
    {
    case Collider::Type::BOX:
        return IsBoxCollision((BoxCollider*)collider);
    case Collider::Type::SPHERE:
        return IsSphereCollision((SphereCollider*)collider);
    case Collider::Type::CAPSULE:
        return IsCapsuleCollision((CapsuleCollider*)collider);
    }

    return false;
}

bool Collider::PushCollision(Collider* collider)
{
    if (!IsCollision(collider)) return false;

    // 충돌 방향 검사
    Vector3 dir = (collider->transform->GlobalPos() - transform->GlobalPos()).GetNormalized();
    // 충돌된 반대 방향으로 밀어냄
    collider->transform->GetParent()->Pos() += dir * PUSH_SPEED * DELTA;

    return true;
}

void Collider::SetColor(Float4 color)
{
    vector<VertexColor>& vertices = mesh->GetVertices();
    
    for (UINT i = 0; i < vertices.size(); i++)
    {
        vertices[i].color = color;
    }
    mesh->UpdateVertex();
}
