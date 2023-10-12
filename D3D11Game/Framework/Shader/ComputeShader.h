#pragma once

// ��ǻƮ ���̴��� �Ϲ� ������ ���������ΰ� ������ �׷��� ī�忡�� ����Ǵ� ���α׷�
// ����ó���� ���� ���̴�
class ComputeShader : public Shader
{
private :
	friend class Shader;

	ComputeShader(wstring filePath);
	~ComputeShader();

private :
	// Shader��(��) ���� ��ӵ�
	virtual void CompileFromFile(wstring filePath) override;
	void CreateShader();

public :
	virtual void SetShader() override;


private :
	ID3D11ComputeShader* computeShader;

};