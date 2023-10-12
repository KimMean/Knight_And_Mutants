#include "Framework.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
    : data(data), dataSize(dataSize)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // �⺻
    bufferDesc.ByteWidth = dataSize;        // DataSize
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // ConstBuffer�� ���
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT result = DEVICE->CreateBuffer(&bufferDesc, nullptr, &buffer);

    if (FAILED(result))
    {
        result = 0;
    }
}

ConstBuffer::~ConstBuffer()
{
    buffer->Release();
}

void ConstBuffer::SetVertexShaderBuffer(UINT slot)
{
    // ���� �����͸� ������Ʈ�ϴ� ��쿡�� UpdateSubresource �޼��带 ���
    //DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);  // �����͸� GPU�� ����
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->VSSetConstantBuffers(slot, 1, &buffer);     // ���� ���̴��� ��� ���۸� ����
}

void ConstBuffer::SetPixelShaderBuffer(UINT slot)
{
    // ���� �����͸� ������Ʈ�ϴ� ��쿡�� UpdateSubresource �޼��带 ���
    //DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);  // �����͸� GPU�� ����
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->PSSetConstantBuffers(slot, 1, &buffer);     // ������ ���̴��� ��� ���۸� ����
}

void ConstBuffer::SetComputeShaderBuffer(UINT slot)
{
    // ���� �����͸� ������Ʈ�ϴ� ��쿡�� UpdateSubresource �޼��带 ���
    //DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->CSSetConstantBuffers(slot, 1, &buffer);     // ��ǻƮ ���̴��� ��� ���۸� ����
}

void ConstBuffer::SetGeometryShaderBuffer(UINT slot)
{
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->GSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetHullShaderBuffer(UINT slot)
{
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->HSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetDomainShaderBuffer(UINT slot)
{
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->DSSetConstantBuffers(slot, 1, &buffer);
}
