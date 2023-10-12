#include "Framework.h"

TextBox::TextBox() : Quad(Vector2(600, 200))
{
	gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
	SetTexture(L"Textures/Icons/TextBox.png");
	gameObject->transform->Pos() = { CENTER_X, 200.0f, 0.0f };
}

TextBox::~TextBox()
{
}

void TextBox::Render()
{
	if (isStart) UpdateText();
	
	Quad::Render();
	Float2 xy = { gameObject->transform->Pos().x, gameObject->transform->Pos().y };
	//string text = "dddddddd\ndddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd";
	Font::Get()->RenderText(str, xy, Float2(500, 150));
}

void TextBox::UpdateText()
{
	curTime += DELTA;
	// 시간별로 한 글자씩 추가
	if (curTime > wordDelay)
	{
		curTime -= wordDelay;
		str += text[charCount];
		str += text[charCount+1];
		charCount+=2;
		if (charCount >= text.length())
		{
			isStart = false;
		}
	}
}

void TextBox::SetText(string text, float delay)
{
	str = "";
	this->text = text;
	wordDelay = delay;
	curTime = 0.0f;
	charCount = 0;
	isStart = true;
}
