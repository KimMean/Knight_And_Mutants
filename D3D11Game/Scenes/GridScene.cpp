#include "Framework.h"
#include "GridScene.h"

GridScene::GridScene()
{
	vertexShader = Shader::AddVertexShader(L"Shaders/Basic/LineColor.hlsl");
	pixelShader = Shader::AddPixelShader(L"Shaders/Basic/LineColor.hlsl");

	//transform = new Transform();
	worldBuffer = new MatrixBuffer();

	mesh = new Mesh<VertexColor>();
	MakeMesh();
	mesh->CreateMesh();
}

GridScene::~GridScene()
{
	delete mesh;
	//delete transform;
	//delete worldBuffer;
}

void GridScene::Update()
{
	//transform->UpdateWorld();
}

void GridScene::PreRender()
{
	//DC->VSSetShader(nullptr, nullptr, 0);
	//DC->PSSetShader(nullptr, nullptr, 0);
	//Environment::Get()->SetDepthEnable(false);	// µ™Ω∫ ¿˚øÎ ≤®≥ı±‚

	//Environment::Get()->SetDepthStencillState();


	//Environment::Get()->SetDepthEnable(true);

	//Environment::Get()->SetDepthStencillState();
}

void GridScene::Render()
{

	Environment::Get()->SetDepthEnable(false);	// µ™Ω∫ ¿˚øÎ ≤®≥ı±‚
	Environment::Get()->SetDepthStencilState();

	worldBuffer->SetVertexShaderBuffer(0);

	vertexShader->SetShader();
	pixelShader->SetShader();

	mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	Environment::Get()->SetDepthEnable(true);	// µ™Ω∫ ¿˚øÎ ≤®≥ı±‚
	Environment::Get()->SetDepthStencilState();
	//transform->SetBuffer();
	//worldBuffer->SetVertexShaderBuffer(0);

	//vector<UINT>& indices = mesh->GetIndices();
	//indices = {
	//	0, 1, 2, 3, 5, 6
	//};
	//DC->DrawIndexed(indices.size(), 0, 0);
}

void GridScene::PostRender()
{
}

void GridScene::GUIRender()
{
}

void GridScene::MakeMesh()
{
	vector<VertexColor>& vertices = mesh->GetVertices();
	//vertices.clear();

	int size = GRID_SIZE * GRID_OFFSET;

	//Axis X
	vertices.emplace_back(-size, 0, 0, COLOR_RED);
	vertices.emplace_back(+size, 0, 0, COLOR_RED);

	//Axis Y
	vertices.emplace_back(0, -size, 0, COLOR_BLUE);
	vertices.emplace_back(0, +size, 0, COLOR_BLUE);

	//Axis Z
	vertices.emplace_back(0, 0, -size, COLOR_GREEN);
	vertices.emplace_back(0, 0, +size, COLOR_GREEN);

	// x ∂Û¿Œ
	for (int n = -size; n < size; n += GRID_OFFSET)
	{
		if (n == 0) continue;
		vertices.emplace_back(n, 0, -size, COLOR_GRAY);
		vertices.emplace_back(n, 0, +size, COLOR_GRAY);

		vertices.emplace_back(-size, 0, n, COLOR_GRAY);
		vertices.emplace_back(+size, 0, n, COLOR_GRAY);
	}

}
