#pragma once

// Domain Shader�� Tessellation Shader���� ������ 
// ���� ����Ʈ(Control Point)�� ��� �����͸� ������, 
// �������� ������Ƽ�긦 �����ϴ� Shader
class DomainShader : public Shader
{
private :
	friend class Shader;

	DomainShader(wstring filePath);
	~DomainShader();
private:
	// Shader��(��) ���� ��ӵ�
	virtual void CompileFromFile(wstring filePath) override;
	void CreateShader();

public :
	virtual void SetShader() override;

private :
	ID3D11DomainShader* domainShader;
};