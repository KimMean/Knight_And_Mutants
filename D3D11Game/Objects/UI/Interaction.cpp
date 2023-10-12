#include "Framework.h"

Interaction::Interaction(Vector2 size) : Quad(size)
{
    gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
    SetTexture(L"Textures/Icons/TextBox.png");
    gameObject->transform->Pos() = { CENTER_X, CENTER_Y };
    gameObject->transform->UpdateWorld();

    icon = new Quad(Vector2(30, 30));
    icon->gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
    icon->SetTexture(L"Textures/Icons/F_Key.png");
    icon->gameObject->transform->SetParent(gameObject->transform);
    icon->gameObject->transform->Pos().x = -50.0f;
}

Interaction::~Interaction()
{
    delete icon;
}

void Interaction::Render()
{
    Quad::Render();
    icon->Render();
    Float2 xy = { gameObject->transform->Pos().x + 25.0f, gameObject->transform->Pos().y };
    Font::Get()->RenderText("대화하기", xy, Float2(100, 10));
}

Quad* Interaction::GetIcon()
{
    return nullptr;
}

