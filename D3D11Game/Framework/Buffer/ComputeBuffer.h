#pragma once

// StructuredBuffer
class ComputeBuffer
{
public :
	ComputeBuffer(void* inputData, UINT inputStride, UINT inputCount,
		UINT outputStride, UINT outputCount);
	~ComputeBuffer();

	void CopyResource(void* data, UINT size);

	void UpdateInput(void* data);

	ID3D11ShaderResourceView*& GetSRV() { return srv; }
	ID3D11UnorderedAccessView*& GetUAV() { return uav; }

	void SetComputeShaderSRV(UINT slot = 0);
	void SetComputeShaderUAV(UINT slot = 0);

private :
	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

private :
	// 입력하는 용도의 버퍼
	ID3D11Buffer* inputBuffer;
	ID3D11ShaderResourceView* srv;

	// GPU 에서 output으로 내보냄
	ID3D11Buffer* outputBuffer;
	ID3D11UnorderedAccessView* uav;

	// 쉐이더에서 카피해온 데이터를 저장하는 장소
	ID3D11Buffer* result;

	void* inputData;

	UINT inputStride, inputCount;
	UINT outputStride, outputCount;
};