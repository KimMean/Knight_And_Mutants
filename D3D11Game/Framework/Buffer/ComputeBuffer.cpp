#include "Framework.h"

ComputeBuffer::ComputeBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount)
    : inputData(inputData), inputStride(inputStride), inputCount(inputCount), outputStride(outputStride), outputCount(outputCount)
{
    CreateInput();
    CreateSRV();
    CreateOutput();
    CreateUAV();
    CreateResult();
}

ComputeBuffer::~ComputeBuffer()
{
    inputBuffer->Release();
    srv->Release();
    outputBuffer->Release();
    uav->Release();
    result->Release();
}

void ComputeBuffer::CopyResource(void* data, UINT size)
{
    // GPU���� ������ �����͸� CPU�� �������� 
    // outputBuffer���� resut�� ����
    DC->CopyResource(result, outputBuffer);

    // ���ҽ� ������ ���� ���Ǵ� ����ü�Դϴ�.
    D3D11_MAPPED_SUBRESOURCE subResource = {};

    DC->Map(
        result, // ������ ���ҽ�
        0,      // ���긮�ҽ� ��ȣ
        D3D11_MAP_READ, // ���� ��� (�б�����)
        0,      // ���� ���ǿ� ���� �ɼ�
        &subResource    // ���ε� ����ü�� ������
    );
    // �����͸� ������
    memcpy(data, subResource.pData, size);
    //  ������ �Ϸ�Ǹ� �ݵ�� Unmap() �Լ��� ȣ���Ͽ� ������ �������־�� �մϴ�.
    DC->Unmap(result, 0);
}

// ������ �޶����� ������Ʈ
void ComputeBuffer::UpdateInput(void* data)
{
    DC->UpdateSubresource(inputBuffer, 0, nullptr, data, inputStride, inputCount);

    srv->Release();

    CreateSRV();
}
void ComputeBuffer::SetComputeShaderSRV(UINT slot)
{
    DC->CSSetShaderResources(slot, 1, &srv);
}
void ComputeBuffer::SetComputeShaderUAV(UINT slot)
{
    DC->CSSetUnorderedAccessViews(slot, 1, &uav, nullptr);
}
// ������ �Է¿� ���� ����
void ComputeBuffer::CreateInput()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = inputStride * inputCount;
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.StructureByteStride = inputStride;
    // bufferDesc.CPUAccessFlags

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = inputData;

    DEVICE->CreateBuffer(&bufferDesc, &data, &inputBuffer);
}

void ComputeBuffer::CreateSRV()
{
    // ��κ� �ؽ�ó�� ���� ������ ��� ����
    D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;  // ID3D11Buffer�� ���� ����
    desc.Buffer.NumElements = inputCount;

    // ShaderResourceView ����
    DEVICE->CreateShaderResourceView(inputBuffer, &desc, &srv);
}

// GPU -> Buffer ���� ����
void ComputeBuffer::CreateOutput()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = outputStride * outputCount;
    bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.StructureByteStride = outputStride;

    DEVICE->CreateBuffer(&bufferDesc, nullptr, &outputBuffer);
}

// UnorderedAccessView ����
void ComputeBuffer::CreateUAV()
{
    // D3D11_SHADER_RESOURCE_VIEW_DESC�� ���������
    // �б�� ���� ��� �����ϴٴ� ��
    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.NumElements = outputCount;

    DEVICE->CreateUnorderedAccessView(outputBuffer, &uavDesc, &uav);
}

// ��� �� ����� Buffer ����
void ComputeBuffer::CreateResult()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    outputBuffer->GetDesc(&bufferDesc); // output�� ������ �� ��������
    bufferDesc.Usage = D3D11_USAGE_STAGING;
    bufferDesc.BindFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    DEVICE->CreateBuffer(&bufferDesc, nullptr, &result);
}
