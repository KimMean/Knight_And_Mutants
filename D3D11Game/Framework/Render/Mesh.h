#pragma once

template<typename T>
class Mesh
{
public :
	Mesh() = default;
	~Mesh();

	void Draw(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void DrawInstanced(UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void CreateMesh();
	void UpdateVertex();
	void UpdateIndex();

	vector<T>& GetVertices() { return vertices; }
	vector<UINT>& GetIndices() { return indices; }

private :
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	vector<T> vertices;
	vector<UINT> indices;
};

template<typename T>
inline Mesh<T>::~Mesh()
{
	delete vertexBuffer;
	delete indexBuffer;
}

template<typename T>
inline void Mesh<T>::Draw(D3D11_PRIMITIVE_TOPOLOGY type)
{
	vertexBuffer->IASet(type);
	
	if (indexBuffer)
	{
		// 인덱스 버퍼가 있을 경우 인덱스를 기준으로 그림
		indexBuffer->IASet();
		DC->DrawIndexed(indices.size(), 0, 0);
	}
	else
	{
		// 인덱스 버퍼가 없을 경우 버텍스 기준
		DC->Draw(vertices.size(), 0);
	}
}

template<typename T>
inline void Mesh<T>::DrawInstanced(UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY type)
{
	vertexBuffer->IASet(type);

	if (indexBuffer)
	{
		indexBuffer->IASet();
		DC->DrawIndexedInstanced(indices.size(), instanceCount, 0, 0, 0);
	}
	else
	{
		DC->DrawInstanced(vertices.size(), instanceCount, 0, 0);
	}
}

template<typename T>
inline void Mesh<T>::CreateMesh()
{
	if (vertices.size() > 0)
	{
		vertexBuffer = new VertexBuffer(vertices.data(), sizeof(T), vertices.size());
	}

	if (indices.size() > 0)
	{
		indexBuffer = new IndexBuffer(indices.data(), indices.size());
	}
}

template<typename T>
inline void Mesh<T>::UpdateVertex()
{
	vertexBuffer->Update(vertices.data(), vertices.size());
}

template<typename T>
inline void Mesh<T>::UpdateIndex()
{
	indexBuffer->Update(indices.data(), indices.size());
}
