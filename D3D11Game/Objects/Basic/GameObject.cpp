#include "Framework.h"

GameObject::GameObject()
{
	//SetVertexShader();
	//SetPixelShader();

	transform = new Transform();
	material = new Material();
	//worldBuffer = new MatrixBuffer();
}

GameObject::~GameObject()
{
	delete transform;
}

void GameObject::SetRender()
{
	SetBuffer();
	SetShader();
}

//void GameObject::SetAllShader(wstring shaderFilePath)
//{
//	SetVertexShader(shaderFilePath);
//	SetPixelShader(shaderFilePath);
//}
//
//void GameObject::SetVertexShader(wstring shaderFilePath)
//{
//	material->SetShaderFile(shaderFilePath);
//	//material.AddVer.vertexShader = Shader::AddVertexShader(shaderFilePath);
//}
//
//void GameObject::SetPixelShader(wstring shaderFilePath)
//{
//	//pixelShader = Shader::AddPixelShader(shaderFilePath);
//}

void GameObject::SetBuffer()
{
	transform->SetBuffer();
	//worldBuffer->SetMatrix(transform->GetWorld());
	//worldBuffer->SetVertexShaderBuffer(0);
}

void GameObject::SetShader()
{
	material->SetMaterial();
	//vertexShader->SetShader();
	//pixelShader->SetShader();
}
