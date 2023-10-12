#include "Framework.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count)
    : stride(stride)
{
    // ���ؽ� ���� ����
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = stride * count; // ���ؽ� 3��
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;   // ���� ����� ����
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // � ���۷� �������
    vertexBufferDesc.CPUAccessFlags = 0;    // D3D11_CPU_ACCESS_WRITE
    vertexBufferDesc.MiscFlags = 0;         // ���ҽ��� �ٸ� �Ӽ��� �����ϴ� �� ���˴ϴ�.

    // ���ؽ� ������ ����
    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = data;              // �ʱ� �����Ͱ� �ִ� �޸��� ������
    vertexData.SysMemPitch = 0;             // �� ������ ����Ʈ ������ ����
    vertexData.SysMemSlicePitch = 0;        // ���긮�ҽ��� ���� �����̽� ������ ����Ʈ ������ ����

    // ���ؽ� ���� ����
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
