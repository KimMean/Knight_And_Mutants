#include "Framework.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
    : data(data), dataSize(dataSize)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // 기본
    bufferDesc.ByteWidth = dataSize;        // DataSize
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // ConstBuffer로 사용
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
    // 작은 데이터를 업데이트하는 경우에는 UpdateSubresource 메서드를 사용
    //DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);  // 데이터를 GPU에 전달
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->VSSetConstantBuffers(slot, 1, &buffer);     // 정점 셰이더의 상수 버퍼를 설정
}

void ConstBuffer::SetPixelShaderBuffer(UINT slot)
{
    // 작은 데이터를 업데이트하는 경우에는 UpdateSubresource 메서드를 사용
    //DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);  // 데이터를 GPU에 전달
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->PSSetConstantBuffers(slot, 1, &buffer);     // 꼭지점 셰이더의 상수 버퍼를 설정
}

void ConstBuffer::SetComputeShaderBuffer(UINT slot)
{
    // 작은 데이터를 업데이트하는 경우에는 UpdateSubresource 메서드를 사용
    //DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->CSSetConstantBuffers(slot, 1, &buffer);     // 컴퓨트 쉐이더의 상수 버퍼를 설정
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
