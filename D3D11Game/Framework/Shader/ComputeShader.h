#pragma once

// 컴퓨트 셰이더는 일반 렌더링 파이프라인과 별도로 그래픽 카드에서 실행되는 프로그램
// 병렬처리를 위한 쉐이더
class ComputeShader : public Shader
{
private :
	friend class Shader;

	ComputeShader(wstring filePath);
	~ComputeShader();

private :
	// Shader을(를) 통해 상속됨
	virtual void CompileFromFile(wstring filePath) override;
	void CreateShader();

public :
	virtual void SetShader() override;


private :
	ID3D11ComputeShader* computeShader;

};