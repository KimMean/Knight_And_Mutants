#pragma once

// 이미지 픽셀을 처리하는 쉐이더
class PixelShader : public Shader
{
public:
	PixelShader(wstring filePath);
	~PixelShader();

private :
	// Shader을(를) 통해 상속됨
	virtual void CompileFromFile(wstring filePath) override;
	void CreateShader();

public:
	virtual void SetShader() override;

private:
	ID3D11PixelShader* pixelShader;

};