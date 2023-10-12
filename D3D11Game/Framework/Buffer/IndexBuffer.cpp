#include "Framework.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(void* data, UINT count)
{
    // 인덱스 버퍼 생성
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.ByteWidth = sizeof(UINT) * count; // 인덱스 3개
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    // 인덱스 데이터 저장
    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = data;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // 인덱스 버퍼 생성
    DEVICE->CreateBuffer(&indexBufferDesc, &indexData, &buffer);
}

IndexBuffer::~IndexBuffer()
{
    buffer->Release();
}

void IndexBuffer::IASet()
{
    DC->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer::Update(void* data, UINT count)
{
    DC->UpdateSubresource(buffer, 0, nullptr, data, sizeof(UINT), count);
}
