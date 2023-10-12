#pragma once

// �̹��� �ȼ��� ó���ϴ� ���̴�
class PixelShader : public Shader
{
public:
	PixelShader(wstring filePath);
	~PixelShader();

private :
	// Shader��(��) ���� ��ӵ�
	virtual void CompileFromFile(wstring filePath) override;
	void CreateShader();

public:
	virtual void SetShader() override;

private:
	ID3D11PixelShader* pixelShader;

};