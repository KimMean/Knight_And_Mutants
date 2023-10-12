#include "Framework.h"

ComputeShader::ComputeShader(wstring filePath)
{
    this->file = file;
    CompileFromFile(filePath);
}

ComputeShader::~ComputeShader()
{
    computeShader->Release();
}

void ComputeShader::CompileFromFile(wstring filePath)
{
    HRESULT result;
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    result = D3DCompileFromFile(
        filePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "CS",
        "cs_5_0",
        flags,
        0,
        &blob,
        nullptr
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED D3DCompileFromFile()");
        return;
    }
    CreateShader();
}

void ComputeShader::CreateShader()
{
    HRESULT result;
    result = DEVICE->CreateComputeShader(
        blob->GetBufferPointer(),    // HLSL 코드에 대한 포인터입니다.
        blob->GetBufferSize(),       // HLSL 코드의 길이입니다.
        nullptr,                     // ID3D11ClassLinkage 인터페이스에 대한 포인터
        &computeShader               // ID3D11ComputeShader 인터페이스 개체에 대한 포인터입니다.
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED CreateComputeShader()");
        return;
    }
}

void ComputeShader::SetShader()
{
    DC->CSSetShader(computeShader, nullptr, 0);
}
