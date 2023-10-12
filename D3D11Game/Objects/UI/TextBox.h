#pragma once

class TextBox : public Quad
{
public :
	TextBox();
	~TextBox();

	void Render();

	void UpdateText();
	void SetText(string text, float delay = 0.05f);

private :
	string text;
	string str;

	float wordDelay;
	float curTime;
	UINT charCount;
public :
	bool isStart = false;
};
