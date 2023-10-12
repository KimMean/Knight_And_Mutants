#pragma once

// 정점 정보를 처리하는 쉐이더
class VertexShader : public Shader
{
private :
	friend class Shader;

	VertexShader(wstring filePath);
	~VertexShader();

private :
	virtual void CompileFromFile(wstring filePath) override;
	void CreateShader();
	void CreateInputLayout();
	DXGI_FORMAT SetInputElementFormat(BYTE mask, D3D_REGISTER_COMPONENT_TYPE type);

public :
	virtual void SetShader() override;

private :
	

private :
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;

	ID3D11ShaderReflection* reflection;
};