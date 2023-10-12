#include "Framework.h"

VertexShader::VertexShader(wstring filePath)
{
    this->file = filePath;
    CompileFromFile(file);
}

VertexShader::~VertexShader()
{
    vertexShader->Release();
    inputLayout->Release();
    reflection->Release();
}

void VertexShader::CompileFromFile(wstring filePath)
{

    HRESULT result;

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    result = D3DCompileFromFile(
        filePath.c_str(),       // 대상 파일 문자열 포인터
        nullptr,                // 전처리기 매크로 정의
        D3D_COMPILE_STANDARD_FILE_INCLUDE,   // 참조하는 다른 파일의 경로
        "VS",                 // 진입점 함수 이름
        "vs_5_0",               // 셰이더 코드의 대상 버전
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


void VertexShader::CreateShader()
{
    // 정점 셰이더 생성
    HRESULT result;
    result = DEVICE->CreateVertexShader(
        blob->GetBufferPointer(),   // HLSL 코드에 대한 포인터입니다.
        blob->GetBufferSize(),      // HLSL 코드의 길이입니다.
        nullptr,                    // ID3D11ClassLinkage 인터페이스에 대한 포인터
        &vertexShader               // ID3D11VertexShader 인터페이스 개체에 대한 포인터입니다.
    );

    if (FAILED(result))
    {
        OutputDebugString(L"Error : FAILED CreateVertexShader()");
        return;
    }

    CreateInputLayout();
}

void VertexShader::CreateInputLayout()
{
    D3DReflect(
        blob->GetBufferPointer(),   // 셰이더 코드의 포인터
        blob->GetBufferSize(),      // 셰이더 코드의 크기
        IID_ID3D11ShaderReflection, // 반환할 인터페이스의 GUID  dxguid.lib 참조
        (void**)&reflection         // 반환된 인터페이스를 저장할 포인터
    );

    D3D11_SHADER_DESC shaderDesc;
    reflection->GetDesc(&shaderDesc);   // 셰이더 자원(Shader Resources) 및 셰이더 입력(Inputs)에 대한 정보 가져오기

    vector<D3D11_INPUT_ELEMENT_DESC> inputLayouts;      
    inputLayouts.reserve(shaderDesc.InputParameters);

    for (UINT i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        reflection->GetInputParameterDesc(i, &paramDesc);       // D3D11_SIGNATURE_PARAMETER_DESC 정보 가져오기

        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName = paramDesc.SemanticName;      // 시멘틱 이름
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;    // 시멘틱 인덱스 POSITION'0'
        elementDesc.InputSlot = 0;                              // 슬롯 번호
        elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;   // 구조체의 시작부터 떨어진 바이트(Byte) 수
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;       // 버텍스 데이터
        elementDesc.InstanceDataStepRate = 0;       // ?

        // 바이트 수 만큼 포맷 크기 설정
        elementDesc.Format = SetInputElementFormat(paramDesc.Mask, paramDesc.ComponentType);

        string temp = paramDesc.SemanticName;

        // POSITION일 경우 Float4로 설정되어 있지만 3개의 값만 쓰기 때문에 변경
        if (temp == "POSITION")
            elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

        int n = temp.find_first_of('_');
        temp = temp.substr(0, n);

        // INSTANCE일 경우 예외처리
        if (temp == "INSTANCE")
        {
            elementDesc.InputSlot = 1;
            elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
            elementDesc.InstanceDataStepRate = 1;
        }

        inputLayouts.push_back(elementDesc);
    }

    HRESULT result = DEVICE->CreateInputLayout(
        inputLayouts.data(),        // D3D11_INPUT_ELEMENT_DESC 구조체 배열의 포인터
        inputLayouts.size(),        // 배열의 길이
        blob->GetBufferPointer(),   // 쉐이더 코드의 바이트코드입니다. 
        blob->GetBufferSize(),      // 바이트 수
        &inputLayout                // 입력 레이아웃의 포인터
    );

    //D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[2];
    //inputLayoutDesc[0].SemanticName = "POSITION";
    //inputLayoutDesc[0].SemanticIndex = 0;
    //inputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    //inputLayoutDesc[0].InputSlot = 0;
    //inputLayoutDesc[0].AlignedByteOffset = 0;
    //inputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    //inputLayoutDesc[0].InstanceDataStepRate = 0;
    //
    //inputLayoutDesc[1].SemanticName = "UV";
    //inputLayoutDesc[1].SemanticIndex = 0;
    //inputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    //inputLayoutDesc[1].InputSlot = 0;
    //inputLayoutDesc[1].AlignedByteOffset = 12;
    //inputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    //inputLayoutDesc[1].InstanceDataStepRate = 0;
    //
    //UINT layoutSize = ARRAYSIZE(inputLayoutDesc);
    //// 인풋 레이아웃 생성
    //HRESULT result = DEVICE->CreateInputLayout(
    //    inputLayoutDesc,        // D3D11_INPUT_ELEMENT_DESC 구조체 배열의 포인터
    //    layoutSize,             // 배열의 길이
    //    blob->GetBufferPointer(),   // 쉐이더 코드의 바이트코드입니다. 
    //    blob->GetBufferSize(),      // 바이트 수
    //    &inputLayout            // 입력 레이아웃의 포인터
    //);
    //if (FAILED(result)) { return; }
}

DXGI_FORMAT VertexShader::SetInputElementFormat(BYTE mask, D3D_REGISTER_COMPONENT_TYPE type)
{
    DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    //Bit    3   2   1   0
    //       X   Y   Z   W
    // mask 수는 요소 개수의 영향을 받음
    // float3 일때 
    // x, y, z (2, 1, 0) 이므로 mask < 8 이며
    // D3D_REGISTER_COMPONENT_TYPE은 D3D_REGISTER_COMPONENT_FLOAT32이다.

    if (mask < 2)
    {
        if (type == D3D_REGISTER_COMPONENT_UINT32)
            format = DXGI_FORMAT_R32_UINT;
        else if (type == D3D_REGISTER_COMPONENT_SINT32)
            format = DXGI_FORMAT_R32_SINT;
        else if (type == D3D_REGISTER_COMPONENT_FLOAT32)
            format = DXGI_FORMAT_R32_FLOAT;
    }
    else if (mask < 4)
    {
        if (type == D3D_REGISTER_COMPONENT_UINT32)
            format = DXGI_FORMAT_R32G32_UINT;
        else if (type == D3D_REGISTER_COMPONENT_SINT32)
            format = DXGI_FORMAT_R32G32_SINT;
        else if (type == D3D_REGISTER_COMPONENT_FLOAT32)
            format = DXGI_FORMAT_R32G32_FLOAT;
    }
    else if (mask < 8)
    {
        if (type == D3D_REGISTER_COMPONENT_UINT32)
            format = DXGI_FORMAT_R32G32B32_UINT;
        else if (type == D3D_REGISTER_COMPONENT_SINT32)
            format = DXGI_FORMAT_R32G32B32_SINT;
        else if (type == D3D_REGISTER_COMPONENT_FLOAT32)
            format = DXGI_FORMAT_R32G32B32_FLOAT;
    }
    else if (mask < 16)
    {
        if (type == D3D_REGISTER_COMPONENT_UINT32)
            format = DXGI_FORMAT_R32G32B32A32_UINT;
        else if (type == D3D_REGISTER_COMPONENT_SINT32)
            format = DXGI_FORMAT_R32G32B32A32_SINT;
        else if (type == D3D_REGISTER_COMPONENT_FLOAT32)
            format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    }
    return format;
}

void VertexShader::SetShader()
{
    DC->IASetInputLayout(inputLayout);             // VertexShader 정보 적용
    DC->VSSetShader(vertexShader, nullptr, 0);     // VertexShader 적용
}
