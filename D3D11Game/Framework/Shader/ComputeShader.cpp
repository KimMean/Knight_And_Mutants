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
        blob->GetBufferPointer(),    // HLSL �ڵ忡 ���� �������Դϴ�.
        blob->GetBufferSize(),       // HLSL �ڵ��� �����Դϴ�.
        nullptr,                     // ID3D11ClassLinkage �������̽��� ���� ������
        &computeShader               // ID3D11ComputeShader �������̽� ��ü�� ���� �������Դϴ�.
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
