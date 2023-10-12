#pragma once

// Domain Shader는 Tessellation Shader에서 생성된 
// 제어 포인트(Control Point)의 출력 데이터를 가지고, 
// 기하학적 프리미티브를 생성하는 Shader
class DomainShader : public Shader
{
private :
	friend class Shader;

	DomainShader(wstring filePath);
	~DomainShader();
private:
	// Shader을(를) 통해 상속됨
	virtual void CompileFromFile(wstring filePath) override;
	void CreateShader();

public :
	virtual void SetShader() override;

private :
	ID3D11DomainShader* domainShader;
};