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
    // GPU에서 생성된 데이터를 CPU로 가져오기 
    // outputBuffer에서 resut로 복사
    DC->CopyResource(result, outputBuffer);

    // 리소스 매핑을 위해 사용되는 구조체입니다.
    D3D11_MAPPED_SUBRESOURCE subResource = {};

    DC->Map(
        result, // 매핑할 리소스
        0,      // 서브리소스 번호
        D3D11_MAP_READ, // 매핑 방식 (읽기전용)
        0,      // 매핑 조건에 대한 옵션
        &subResource    // 매핑된 구조체의 포인터
    );
    // 데이터를 복사함
    memcpy(data, subResource.pData, size);
    //  매핑이 완료되면 반드시 Unmap() 함수를 호출하여 매핑을 해제해주어야 합니다.
    DC->Unmap(result, 0);
}

// 데이터 달라지면 업데이트
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
// 데이터 입력용 버퍼 생성
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
    // 대부분 텍스처에 대한 정보를 담고 있음
    D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;  // ID3D11Buffer를 위한 설정
    desc.Buffer.NumElements = inputCount;

    // ShaderResourceView 생성
    DEVICE->CreateShaderResourceView(inputBuffer, &desc, &srv);
}

// GPU -> Buffer 정보 전달
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

// UnorderedAccessView 생성
void ComputeBuffer::CreateUAV()
{
    // D3D11_SHADER_RESOURCE_VIEW_DESC와 비슷하지만
    // 읽기와 쓰기 모두 가능하다는 점
    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.NumElements = outputCount;

    DEVICE->CreateUnorderedAccessView(outputBuffer, &uavDesc, &uav);
}

// 결과 값 복사용 Buffer 생성
void ComputeBuffer::CreateResult()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    outputBuffer->GetDesc(&bufferDesc); // output에 설정된 값 가져오기
    bufferDesc.Usage = D3D11_USAGE_STAGING;
    bufferDesc.BindFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    DEVICE->CreateBuffer(&bufferDesc, nullptr, &result);
}
