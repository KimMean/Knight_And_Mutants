#pragma once

// Hull Shader�� �Էµ� ����(Vertex) �����͸� ������,
// Tessellation ó���� �ʿ��� ���� ����Ʈ(Control Point)�� �����ϴ� Shader
class HullShader : public Shader
{
private :
	friend class Shader;

	HullShader(wstring filePath);
	~HullShader();
private:
	// Shader��(��) ���� ��ӵ�
	virtual void CompileFromFile(wstring filePath) override;
	void CreateShader();

public :
	virtual void SetShader() override;

private :
	ID3D11HullShader* hullShader;
};