#include "Framework.h"
#include "HullShader.h"

HullShader::HullShader(wstring filePath)
{
    this->file = file;
    CompileFromFile(filePath);
}

HullShader::~HullShader()
{
    hullShader->Release();
}

void HullShader::CompileFromFile(wstring filePath)
{
    HRESULT result;
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    result = D3DCompileFromFile(
        filePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "HS",
        "hs_5_0",
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

void HullShader::CreateShader()
{
    HRESULT result;
    result = DEVICE->CreateHullShader(
        blob->GetBufferPointer(),    // HLSL �ڵ忡 ���� �������Դϴ�.
        blob->GetBufferSize(),       // HLSL �ڵ��� �����Դϴ�.
        nullptr,                     // ID3D11ClassLinkage �������̽��� ���� ������
        &hullShader               // ID3D11ComputeShader �������̽� ��ü�� ���� �������Դϴ�.
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED CreateHullShader()");
        return;
    }
}

void HullShader::SetShader()
{
    DC->HSSetShader(hullShader, nullptr, 0);
}
