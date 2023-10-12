#include "Framework.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count)
    : stride(stride)
{
    // 버텍스 버퍼 생성
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = stride * count; // 버텍스 3개
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;   // 동작 방식을 지정
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 어떤 버퍼로 사용할지
    vertexBufferDesc.CPUAccessFlags = 0;    // D3D11_CPU_ACCESS_WRITE
    vertexBufferDesc.MiscFlags = 0;         // 리소스의 다른 속성을 지정하는 데 사용됩니다.

    // 버텍스 데이터 저장
    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = data;              // 초기 데이터가 있는 메모리의 포인터
    vertexData.SysMemPitch = 0;             // 행 간격을 바이트 단위로 지정
    vertexData.SysMemSlicePitch = 0;        // 서브리소스의 깊이 슬라이스 간격을 바이트 단위로 지정

    // 버텍스 버퍼 생성
    DEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &buffer);
}

VertexBuffer::~VertexBuffer()
{
    buffer->Release();
}

void VertexBuffer::IASet(D3D11_PRIMITIVE_TOPOLOGY type)
{
    DC->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    DC->IASetPrimitiveTopology(type);
}

void VertexBuffer::IASet(UINT slot, D3D11_PRIMITIVE_TOPOLOGY type)
{
    DC->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
    DC->IASetPrimitiveTopology(type);
}

void VertexBuffer::Update(void* data, UINT count)
{
    DC->UpdateSubresource(buffer, 0, nullptr, data, stride, count);
}
