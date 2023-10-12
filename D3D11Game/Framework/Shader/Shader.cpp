#include "Framework.h"

unordered_map<wstring, Shader*> Shader::shaders;

Shader::Shader()
{
}

Shader::~Shader()
{
    blob->Release();
}

void Shader::Delete()
{
    for (pair<wstring, Shader*> shader : shaders)
        delete shader.second;
}

VertexShader* Shader::AddVertexShader(wstring file)
{
    if (file.size() == 0) return nullptr;
    
    wstring key = GetFileName(file) + L"VS";

    if (shaders.count(key) > 0)
        return (VertexShader*)shaders[key];

    shaders[key] = new VertexShader(file);

    return (VertexShader*)shaders[key];
}

PixelShader* Shader::AddPixelShader(wstring file)
{
    if (file.size() == 0) return nullptr;

    wstring key = GetFileName(file) + L"PS";

    if (shaders.count(key) > 0)
        return (PixelShader*)shaders[key];

    shaders[key] = new PixelShader(file);

    return (PixelShader*)shaders[key];
}

ComputeShader* Shader::AddComputeShader(wstring file)
{
    if (file.size() == 0) return nullptr;

    wstring key = GetFileName(file) + L"CS";

    if (shaders.count(key) > 0)
        return (ComputeShader*)shaders[key];

    shaders[key] = new ComputeShader(file);

    return (ComputeShader*)shaders[key];
}

//void Shader::AddShader(ShaderType type, wstring filePath)
//{
    //D3DCompileFromFile(,)
    //switch (type)
    //{
    //case Shader::VERTEX:
    //    shaders[L"key"] = new VertexShader(file);
    //    break;
    //case Shader::PIXEL:
    //    break;
    //default:
    //    break;
    //}

    //HRESULT result;
    //result = D3DCompileFromFile(
    //    L"Shader/VertexShader.hlsl",    // ��� ���� ���ڿ� ������
    //    nullptr,                        // ��ó���� ��ũ�� ����
    //    nullptr,                        // �����ϴ� �ٸ� ������ ���
    //    "main",                         // ������ �Լ� �̸�
    //    "vs_5_0",                       // ���̴� �ڵ��� ��� ����
    //    D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,    // ������ �ɼ�
    //    0,                              // ����Ǿ� ���� �׻� 0?
    //    &blob,              // �����ϵ� ���̴� �ڵ尡 ����� ��ġ
    //    nullptr                         // ������ ���� �޽��� ���� ��ġ
    //);
//}

