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
        filePath.c_str(),       // ��� ���� ���ڿ� ������
        nullptr,                // ��ó���� ��ũ�� ����
        D3D_COMPILE_STANDARD_FILE_INCLUDE,   // �����ϴ� �ٸ� ������ ���
        "VS",                 // ������ �Լ� �̸�
        "vs_5_0",               // ���̴� �ڵ��� ��� ����
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


void VertexShader::CreateShader()
{
    // ���� ���̴� ����
    HRESULT result;
    result = DEVICE->CreateVertexShader(
        blob->GetBufferPointer(),   // HLSL �ڵ忡 ���� �������Դϴ�.
        blob->GetBufferSize(),      // HLSL �ڵ��� �����Դϴ�.
        nullptr,                    // ID3D11ClassLinkage �������̽��� ���� ������
        &vertexShader               // ID3D11VertexShader �������̽� ��ü�� ���� �������Դϴ�.
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
        blob->GetBufferPointer(),   // ���̴� �ڵ��� ������
        blob->GetBufferSize(),      // ���̴� �ڵ��� ũ��
        IID_ID3D11ShaderReflection, // ��ȯ�� �������̽��� GUID  dxguid.lib ����
        (void**)&reflection         // ��ȯ�� �������̽��� ������ ������
    );

    D3D11_SHADER_DESC shaderDesc;
    reflection->GetDesc(&shaderDesc);   // ���̴� �ڿ�(Shader Resources) �� ���̴� �Է�(Inputs)�� ���� ���� ��������

    vector<D3D11_INPUT_ELEMENT_DESC> inputLayouts;      
    inputLayouts.reserve(shaderDesc.InputParameters);

    for (UINT i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        reflection->GetInputParameterDesc(i, &paramDesc);       // D3D11_SIGNATURE_PARAMETER_DESC ���� ��������

        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName = paramDesc.SemanticName;      // �ø�ƽ �̸�
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;    // �ø�ƽ �ε��� POSITION'0'
        elementDesc.InputSlot = 0;                              // ���� ��ȣ
        elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;   // ����ü�� ���ۺ��� ������ ����Ʈ(Byte) ��
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;       // ���ؽ� ������
        elementDesc.InstanceDataStepRate = 0;       // ?

        // ����Ʈ �� ��ŭ ���� ũ�� ����
        elementDesc.Format = SetInputElementFormat(paramDesc.Mask, paramDesc.ComponentType);

        string temp = paramDesc.SemanticName;

        // POSITION�� ��� Float4�� �����Ǿ� ������ 3���� ���� ���� ������ ����
        if (temp == "POSITION")
            elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

        int n = temp.find_first_of('_');
        temp = temp.substr(0, n);

        // INSTANCE�� ��� ����ó��
        if (temp == "INSTANCE")
        {
            elementDesc.InputSlot = 1;
            elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
            elementDesc.InstanceDataStepRate = 1;
        }

        inputLayouts.push_back(elementDesc);
    }

    HRESULT result = DEVICE->CreateInputLayout(
        inputLayouts.data(),        // D3D11_INPUT_ELEMENT_DESC ����ü �迭�� ������
        inputLayouts.size(),        // �迭�� ����
        blob->GetBufferPointer(),   // ���̴� �ڵ��� ����Ʈ�ڵ��Դϴ�. 
        blob->GetBufferSize(),      // ����Ʈ ��
        &inputLayout                // �Է� ���̾ƿ��� ������
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
    //// ��ǲ ���̾ƿ� ����
    //HRESULT result = DEVICE->CreateInputLayout(
    //    inputLayoutDesc,        // D3D11_INPUT_ELEMENT_DESC ����ü �迭�� ������
    //    layoutSize,             // �迭�� ����
    //    blob->GetBufferPointer(),   // ���̴� �ڵ��� ����Ʈ�ڵ��Դϴ�. 
    //    blob->GetBufferSize(),      // ����Ʈ ��
    //    &inputLayout            // �Է� ���̾ƿ��� ������
    //);
    //if (FAILED(result)) { return; }
}

DXGI_FORMAT VertexShader::SetInputElementFormat(BYTE mask, D3D_REGISTER_COMPONENT_TYPE type)
{
    DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    //Bit    3   2   1   0
    //       X   Y   Z   W
    // mask ���� ��� ������ ������ ����
    // float3 �϶� 
    // x, y, z (2, 1, 0) �̹Ƿ� mask < 8 �̸�
    // D3D_REGISTER_COMPONENT_TYPE�� D3D_REGISTER_COMPONENT_FLOAT32�̴�.

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
    DC->IASetInputLayout(inputLayout);             // VertexShader ���� ����
    DC->VSSetShader(vertexShader, nullptr, 0);     // VertexShader ����
}
