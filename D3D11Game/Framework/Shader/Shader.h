#pragma once

class Shader
{
	enum ShaderType
	{
		VERTEX,
		PIXEL
	};

public:
	Shader();
	~Shader();
	
	static void Delete();
	
public :
	static class VertexShader* AddVertexShader(wstring file);
	static class PixelShader* AddPixelShader(wstring file);
	static class ComputeShader* AddComputeShader(wstring file);

	virtual void SetShader() = 0;

	wstring GetName() { return file; }
private :
	virtual void CompileFromFile(wstring filePath) = 0;

protected:
	wstring file;

	ID3DBlob* blob;

	static unordered_map<wstring, Shader*> shaders;
};
