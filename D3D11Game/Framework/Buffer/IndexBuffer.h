#pragma once

class IndexBuffer
{
public :
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

public :
	void IASet();

	void Update(void* data, UINT count);
private :
	ID3D11Buffer* buffer;

};