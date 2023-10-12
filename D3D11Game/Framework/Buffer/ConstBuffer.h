#pragma once

class ConstBuffer
{
public :
	ConstBuffer(void* data, UINT dataSize);
	~ConstBuffer();

	void SetVertexShaderBuffer(UINT slot);
	void SetPixelShaderBuffer(UINT slot);
	void SetComputeShaderBuffer(UINT slot);
	void SetGeometryShaderBuffer(UINT slot);
	void SetHullShaderBuffer(UINT slot);
	void SetDomainShaderBuffer(UINT slot);

private :
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;

	D3D11_MAPPED_SUBRESOURCE subResource;
};