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
        filePath.c_str(),       // 대상 파일 문자열 포인터
        nullptr,                // 전처리기 매크로 정의
        D3D_COMPILE_STANDARD_FILE_INCLUDE,  // 참조하는 다른 파일의 경로
        "PS",                 // 진입점 함수 이름
        "ps_5_0",               // 셰이더 코드의 대상 버전
        flags,                  // 컴파일 옵션
        0,                      // 예약되어 있음 항상 0?
        &blob,                  // 컴파일된 쉐이더 코드가 저장될 위치
        nullptr                 // 컴파일 에러 메시지 저장 위치
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
    // 정점 셰이더 생성
    HRESULT result = DEVICE->CreatePixelShader(
        blob->GetBufferPointer(),   // HLSL 코드에 대한 포인터입니다.
        blob->GetBufferSize(),      // HLSL 코드의 길이입니다.
        nullptr,                    // ID3D11ClassLinkage 인터페이스에 대한 포인터
        &pixelShader               // ID3D11PixelShader 인터페이스 개체에 대한 포인터입니다.
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED CreateVertexShader()");
        return;
    }
}


void PixelShader::SetShader()
{
    DC->PSSetShader(pixelShader, nullptr, 0);      // PixelShader 적용
}
