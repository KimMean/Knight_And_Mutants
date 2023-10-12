#include "Framework.h"

PixelShader::PixelShader(wstring filePath)
{
    this->file = filePath;
    CompileFromFile(filePath);
}

PixelShader::~PixelShader()
{
    pixelShader->Release();
}

void PixelShader::CompileFromFile(wstring filePath)
{
    HRESULT result;

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    result = D3DCompileFromFile(
        filePath.c_str(),       // ��� ���� ���ڿ� ������
        nullptr,                // ��ó���� ��ũ�� ����
        D3D_COMPILE_STANDARD_FILE_INCLUDE,  // �����ϴ� �ٸ� ������ ���
        "PS",                 // ������ �Լ� �̸�
        "ps_5_0",               // ���̴� �ڵ��� ��� ����
        flags,                  // ������ �ɼ�
        0,                      // ����Ǿ� ���� �׻� 0?
        &blob,                  // �����ϵ� ���̴� �ڵ尡 ����� ��ġ
        nullptr                 // ������ ���� �޽��� ���� ��ġ
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED D3DCompileFromFile()");
        return;
    }
    CreateShader();
}

void PixelShader::CreateShader()
{
    // ���� ���̴� ����
    HRESULT result = DEVICE->CreatePixelShader(
        blob->GetBufferPointer(),   // HLSL �ڵ忡 ���� �������Դϴ�.
        blob->GetBufferSize(),      // HLSL �ڵ��� �����Դϴ�.
        nullptr,                    // ID3D11ClassLinkage �������̽��� ���� ������
        &pixelShader               // ID3D11PixelShader �������̽� ��ü�� ���� �������Դϴ�.
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED CreateVertexShader()");
        return;
    }
}


void PixelShader::SetShader()
{
    DC->PSSetShader(pixelShader, nullptr, 0);      // PixelShader ����
}
