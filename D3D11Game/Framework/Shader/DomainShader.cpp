#include "Framework.h"

DomainShader::DomainShader(wstring filePath)
{
    this->file = file;
    CompileFromFile(filePath);
}

DomainShader::~DomainShader()
{
    domainShader->Release();
}

void DomainShader::CompileFromFile(wstring filePath)
{
    HRESULT result;
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    result = D3DCompileFromFile(
        filePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "DS",
        "ds_5_0",
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

void DomainShader::CreateShader()
{
    HRESULT result;
    result = DEVICE->CreateDomainShader(
        blob->GetBufferPointer(),    // HLSL �ڵ忡 ���� �������Դϴ�.
        blob->GetBufferSize(),       // HLSL �ڵ��� �����Դϴ�.
        nullptr,                     // ID3D11ClassLinkage �������̽��� ���� ������
        &domainShader               // ID3D11ComputeShader �������̽� ��ü�� ���� �������Դϴ�.
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED CreateDomainShader()");
        return;
    }
}

void DomainShader::SetShader()
{
    DC->DSSetShader(domainShader, nullptr, 0);
}
