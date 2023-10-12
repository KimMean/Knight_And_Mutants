#pragma once

// Hull Shader는 입력된 정점(Vertex) 데이터를 가지고,
// Tessellation 처리에 필요한 제어 포인트(Control Point)를 생성하는 Shader
class HullShader : public Shader
{
private :
	friend class Shader;

	HullShader(wstring filePath);
	~HullShader();
private:
	// Shader을(를) 통해 상속됨
	virtual void CompileFromFile(wstring filePath) override;
	void CreateShader();

public :
	virtual void SetShader() override;

private :
	ID3D11HullShader* hullShader;
};