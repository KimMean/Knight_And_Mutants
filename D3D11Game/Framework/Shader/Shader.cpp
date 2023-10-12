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
    //    L"Shader/VertexShader.hlsl",    // 대상 파일 문자열 포인터
    //    nullptr,                        // 전처리기 매크로 정의
    //    nullptr,                        // 참조하는 다른 파일의 경로
    //    "main",                         // 진입점 함수 이름
    //    "vs_5_0",                       // 셰이더 코드의 대상 버전
    //    D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,    // 컴파일 옵션
    //    0,                              // 예약되어 있음 항상 0?
    //    &blob,              // 컴파일된 쉐이더 코드가 저장될 위치
    //    nullptr                         // 컴파일 에러 메시지 저장 위치
    //);
//}

