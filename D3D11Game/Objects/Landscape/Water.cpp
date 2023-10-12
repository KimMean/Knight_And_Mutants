#include "Framework.h"

Water::Water()
{
    gameObject = new GameObject;
    gameObject->material->SetShaderFile(L"Shaders/Landscape/Water.hlsl");
    gameObject->transform->Pos() = { -32.0f, 3.0f, -32.0f };
    gameObject->transform->UpdateWorld();

    //reflection = new Reflection(this->gameObject->transform);

    //gameObject->GetMaterial()->SetShaderFile(L"Shaders/Light/Light.hlsl");
    mesh = new Mesh<VertexUV>();
    waterMap = Texture::AddTexture(L"Textures/Landscape/water.png");
    waterNormal = Texture::AddTexture(L"Textures/Landscape/WaveNormal.png");
    waterBuffer = new WaterBuffer();
    waterSize = Vector2(128, 128);
	MakeMesh2();
    mesh->CreateMesh();
}

Water::~Water()
{
	delete gameObject;
	delete mesh;
    //delete reflection;
}

void Water::Update()
{
    //reflection->Update();

    curTime += DELTA;
    //if (curTime > 1.0f) curTime -= 1.0f;
    waterBuffer->GetWaterData().timeScale = curTime;

    vector<VertexUV>& vertices = mesh->GetVertices();

    UINT width = waterSize.x;
    UINT height = waterSize.y;
    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            // uv는 왼쪽 위부터니까
            vertices[width * z + x].pos.y = (cos(x + curTime) + cos(z + curTime)) * waveScale;
        }
    }
    mesh->UpdateVertex();

    gameObject->transform->UpdateWorld();
}

void Water::Render()
{
    //reflection->SetRender();    // 버텍스 쉐이더에 반사 뷰, 픽셀 쉐이더에 렌더 결과 셋팅

    Environment::Get()->SetAlphaBlend(true);
    Environment::Get()->SetBlendState();
    gameObject->SetRender();
    waterMap->SetPixelShaderSlot(0);
    waterNormal->SetPixelShaderSlot(2);
    waterBuffer->SetPixelShaderBuffer(10);
    mesh->Draw();
    Environment::Get()->SetAlphaBlend(false);
    Environment::Get()->SetBlendState();
    //Environment::Get()->SetAlphaBlend(false);
    //Environment::Get()->SetAlphaToCoverage(false);
    //Environment::Get()->SetBlendState();
}

void Water::GUIRender()
{
    ImGui::SliderFloat2("XYOffset", (float*)&waterBuffer->GetWaterData().moveScale, 0.001f, 10.0f);
    ImGui::SliderFloat("Wave", (float*)&waterBuffer->GetWaterData().waveScale, 0.001f, 10.0f);
    ImGui::SliderFloat("WaveScale", (float*)&waveScale, 0.001f, 10.0f);
}

void Water::PostRender()
{
    //reflection->PostRender();
}

void Water::SetReflection()
{
    //reflection->SetReflection();
}

void Water::MakeMesh()
{
    float left = -waterSize.x * 0.5f;
    float right = +waterSize.x * 0.5f;
    float top = +waterSize.y * 0.5f;
    float bottom = -waterSize.y * 0.5f;

    vector<VertexUV>& vertices = mesh->GetVertices();

    vertices.emplace_back(left, 0, top, 0, 0);
    vertices.emplace_back(right, 0, top, 1, 0);
    vertices.emplace_back(left, 0, bottom, 0, 1);
    vertices.emplace_back(right, 0, bottom, 1, 1);

    vector<UINT>& indices = mesh->GetIndices();
    indices = { 0, 1, 2, 2, 1, 3 };
}

void Water::MakeMesh2()
{
    UINT width = waterSize.x;
    UINT height = waterSize.y;

    vector<VertexUV>& vertices = mesh->GetVertices();

    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            // uv는 왼쪽 위부터니까
            VertexUV vertex;
            vertex.pos = { (float)x, 0.0f, (float)(height - z) };

            vertex.uv.x = x / (float)width;
            vertex.uv.y = z / (float)height;

            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve(mesh->GetVertices().size() * 6);

    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            indices.push_back((width + 1) * z + x); // 0
            indices.push_back((width + 1) * z + x + 1);//1
            indices.push_back((width + 1) * (z + 1) + x);//2            

            indices.push_back((width + 1) * (z + 1) + x);//2
            indices.push_back((width + 1) * z + x + 1);//1            
            indices.push_back((width + 1) * (z + 1) + x + 1);//3
        }
    }
}
