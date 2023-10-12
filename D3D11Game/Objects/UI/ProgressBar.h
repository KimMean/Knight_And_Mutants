#pragma once

class ProgressBar : public Quad
{
public :
	ProgressBar(Vector2 size);
	~ProgressBar();

	void Update();
	void Render();

	void SetRender();
	void SetAmount(float value);

	void SetBackImage(Vector2 size, wstring textureFile);
	void SetFrontImage(Vector2 size, wstring textureFile);
	Quad* GetFrontImage() { return frontImage; }

public :
	Quad* frontImage;
private :
	FloatValueBuffer* valueBuffer;

	float fillAmount = 1.0f;
};