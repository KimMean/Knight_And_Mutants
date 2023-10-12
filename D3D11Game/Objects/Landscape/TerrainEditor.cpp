#include "Framework.h"

TerrainEditor::TerrainEditor()
{
    gameObject = new GameObject();
    gameObject->material->SetShaderFile(L"Shaders/Landscape/TerrainEditor.hlsl");
    
    gameObject->material->SetDiffuseMap(L"Textures/Landscape/sand.png");
    // BlendTexture
    sandTexture = Texture::AddTexture(L"Textures/Landscape/sand.png");
    grassTexture = Texture::AddTexture(L"Textures/Landscape/grassy.png");
    soilTexture = Texture::AddTexture(L"Textures/Landscape/soil.png");

    mesh = new Mesh<VertexType>();

    computeShader = Shader::AddComputeShader(L"Shaders/Compute/ComputePicking.hlsl");

    brushBuffer = new BrushBuffer();
    rayBuffer = new RayBuffer();

    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

TerrainEditor::~TerrainEditor()
{
    delete gameObject;
    delete mesh;

    delete brushBuffer;
    delete rayBuffer;

    delete computeBuffer;
}

void TerrainEditor::Update()
{
    if (isLoadComplite && ComputePicking())
    {
        brushBuffer->Get().pickingPos = pickingPos;
        if (KEY_PRESS(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
        {
            AdjustHeight();
        }
    }
    if (KEY_UP(VK_LBUTTON))
    {
        UpdateHeight();
    }
}

void TerrainEditor::Render()
{
    gameObject->SetRender();

    sandTexture->SetPixelShaderSlot(10);
    grassTexture->SetPixelShaderSlot(11);
    soilTexture->SetPixelShaderSlot(12);

    brushBuffer->SetPixelShaderBuffer(10);
    //Environment::Get()->Set
    mesh->Draw();
}

void TerrainEditor::GUIRender()
{
    if (ImGui::TreeNode("TerrainEditor"))
    {
        if (ImGui::TreeNode("Brush"))
        {
            const char* list[] = { "FLAT", "DIG", "BUILD" };
            ImGui::Combo("Type", (int*)&adjustType, list, 3);

            ImGui::SliderFloat("Range", &(brushBuffer->Get().range), 0.1f, 100.0f);
            ImGui::SliderFloat("Adjust", &adjustValue, 0.1f, 100.0f);
            ImGui::TreePop();
        }
        SaveHeightMap();
        ImGui::SameLine();
        LoadHeightMap();

        SaveAlphaMap();
        ImGui::SameLine();
        LoadAlphaMap();
        ImGui::TreePop();
    }
}

bool TerrainEditor::ComputePicking()
{
    Ray ray = CAM->ScreenPointToRay(mousePos);

    rayBuffer->Get().pos = ray.pos;
    rayBuffer->Get().dir = ray.dir;
    rayBuffer->Get().triangleSize = triangleSize;

    // 컴퓨트 쉐이더에 레이정보 입력
    rayBuffer->SetComputeShaderBuffer(0);

    computeBuffer->SetComputeShaderSRV(0);   // 입력 쉐이더 리소스 뷰
    computeBuffer->SetComputeShaderUAV(0);   // 출력 정렬되지 않은 액세스 뷰

    computeShader->SetShader();

    //ceil함수는 무조건 바로위 정수로 올림
    // ComputeShader에서 설정한 Thread 개수로 그룹 개수 맞춰야 함
    UINT x = ceil((float)triangleSize / 64.0f);
    DC->Dispatch(x, 1, 1);  // 그룹개수 맞춰서 실행
    // 컴퓨트 쉐이더 처리 후 값 복사
    computeBuffer->CopyResource(
        outputs.data(),                     // 복사받을 장소
        sizeof(OutputDesc) * triangleSize   // 복사 크기
    );

    float minDistance = FLT_MAX;
    int minIndex = -1;
    UINT index = 0;
    // compute 쉐이더에서 처리된 폴리곤들을 돌면서 가장 앞에 충돌된 삼각형의 인덱스를 가져옴 
    for (OutputDesc output : outputs)
    {
        if (output.picked)
        {
            if (minDistance > output.distance)
            {
                minDistance = output.distance;
                minIndex = index;
            }
        }
        index++;
    }

    // 충돌된 삼각형의 position을 가져옴
    if (minIndex >= 0)
    {
        pickingPos = ray.pos + ray.dir * minDistance;
        return true;
    }

    return false;
}

void TerrainEditor::CreateTerrain(Vector2 size)
{
    terrainSize = size;

    MakeMesh();
    MakeNormalVector();
    MakeTangentVector();
    MakeComputeData();
    mesh->CreateMesh();

    computeBuffer = new ComputeBuffer(
        inputs.data(),      // 데이터 시작점
        sizeof(InputDesc),  // 데이터 크기
        triangleSize,       // 데이터 개수
        sizeof(OutputDesc), // 출력데이터 크기
        triangleSize        // 출력 데이터 개수
    );

}


void TerrainEditor::Resize()
{
    isLoadComplite = false;
    MakeMesh();
    MakeNormalVector();
    MakeTangentVector();
    MakeComputeData();
    UpdateBlendTexture();

    mesh->UpdateVertex();
    mesh->UpdateIndex();
    computeBuffer->UpdateInput(inputs.data());

    isLoadComplite = true;
}

// 터레인 높이 조절
void TerrainEditor::AdjustHeight()
{
    vector<VertexType>& vertices = mesh->GetVertices();

    UINT width = terrainSize.x + 1;
    UINT height = terrainSize.y + 1;

    float minH = FLT_MAX;
    float maxH = -FLT_MAX;

    float range = brushBuffer->Get().range;
    pickingPos.y = 0.0f;
    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            int idx = width * z + x;
            Vector3 pos = Vector3(vertices[idx].pos.x, 0, vertices[idx].pos.z);
            float distance = Distance(pos, pickingPos);
            float h = vertices[idx].pos.y;

            // 범위 안에 들어와 있는 정점 높이 조절
            if (distance <= range)
            {
                // 거리별 가중치
                float weight = 1.0f - (distance / range);

                switch (adjustType)
                {
                case TerrainEditor::FLAT:   // 평탄화는 따로 작업
                    if (minH > h) minH = h;
                    if (maxH < h) maxH = h;
                    break;
                case TerrainEditor::DIG:
                    weight *= adjustValue;
                    vertices[idx].pos.y -= weight * DELTA;
                    break;
                case TerrainEditor::BUILD:
                    weight *= adjustValue;
                    vertices[idx].pos.y += weight * DELTA;
                    break;
                }
                //vertex.pos.y += adjustValue * DELTA;
                vertices[idx].pos.y = Clamp(MIN_HEIGHT, MAX_HEIGHT, vertices[idx].pos.y); // 최소 최대 사이로 조절

            }
        }
    }

    float average = (minH + maxH) * 0.5f;
    if (adjustType == FLAT)
    {
        for (VertexType& vertex : vertices)
        {
            Vector3 pos = Vector3(vertex.pos.x, vertex.pos.y, vertex.pos.z);
            float distance = Distance(pos, pickingPos);
            if (distance <= range)
            {
                // 가중치와 단차 구해서 조절
                float weight = 1.0f - (distance / range);
                float step = average - pos.y;
                step *= weight;
                vertex.pos.y += step;
            }
        }
    }
    mesh->UpdateVertex();
    UpdateBlendTexture();
}

// 조절 후 노말 탄젠트 등 업뎃
void TerrainEditor::UpdateHeight()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    for (VertexType& vertex : vertices)
    {
        vertex.normal = {};
        vertex.tangent = {};
    }

    MakeNormalVector();
    MakeTangentVector();
    UpdateBlendTexture();

    mesh->UpdateVertex();
    MakeComputeData();
    computeBuffer->UpdateInput(inputs.data());
}

void TerrainEditor::UpdateBlendTexture()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    for (VertexType& vertex : vertices)
    {
        vertex.alpha[0] = 0;
        vertex.alpha[1] = 0;
        vertex.alpha[2] = 0;
        float heightLevel = (vertex.pos.y - MIN_HEIGHT) / (MAX_HEIGHT - MIN_HEIGHT);
        if(heightLevel < 0.5f)
        {
            vertex.alpha[0] = 0.6f;
        }
        if (heightLevel > 0.4f && heightLevel < 0.7f)
        {
            vertex.alpha[1] = 0.6f;
        }
        if(heightLevel > 0.6f)
        {
            vertex.alpha[2] = 0.6f;
        }
    }
}

void TerrainEditor::AdjustBlendTexture()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    
    pickingPos.y = 0.0f;
    float range = brushBuffer->Get().range;

    for (VertexType& vertex : vertices)
    {
        Vector3 pos = Vector3(vertex.pos.x, 0, vertex.pos.z);

        float distance = Distance(pos, pickingPos);

        float temp = adjustValue * max(0, cos(distance / range));

        if (distance <= range)
        {
            vertex.alpha[selectMap] += temp * DELTA;
            vertex.alpha[selectMap] = Clamp(0.0f, 1.0f, vertex.alpha[selectMap]);
        }
    }
}

void TerrainEditor::MakeMesh()
{
    vector<Float4> pixels(terrainSize.x * terrainSize.y, Float4(0, 0, 0, 0));

    if (heightMap)
    {
        terrainSize.x = (UINT)heightMap->GetSize().x - 1;
        terrainSize.y = (UINT)heightMap->GetSize().y - 1;

        heightMap->ReadPixels(pixels);
    }

    // 칸수 맞추기 위해 + 1
    UINT width = terrainSize.x;
    UINT height = terrainSize.y;

    //Vertices
    vector<VertexType>& vertices = mesh->GetVertices();
    vertices.clear();

    // 왼쪽 위에서부터 uv값과 맞춰서 정점을 찍음
    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            // uv는 왼쪽 위부터니까
            VertexType vertex;
            vertex.pos = { (float)x, 0.0f, (float)(height - z) };

            vertex.uv.x = x / (float)((width) * 0.1f);
            vertex.uv.y = z / (float)((height) * 0.1f);
            if(heightMap)
            {
                UINT index = (width+1) * z + x;
                vertex.pos.y = pixels[index].x * (MAX_HEIGHT - MIN_HEIGHT) + MIN_HEIGHT;
            }

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

void TerrainEditor::MakeNormalVector()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT> indices = mesh->GetIndices();

    UINT index0, index1, index2;
    Vector3 p0, p1, p2;
    Vector3 e0, e1;
    Vector3 normal;

    for (UINT i = 0; i < indices.size() / 3; i++)
    {
        // 3개의 정점 인덱스를 구함
        index0 = indices[i * 3 + 0];
        index1 = indices[i * 3 + 1];
        index2 = indices[i * 3 + 2];

        // 3개 정점의 위치를 받아옴
        p0 = vertices[index0].pos;
        p1 = vertices[index1].pos;
        p2 = vertices[index2].pos;

        // 계산하기 편하게 원점으로 부터 떨어진 position을 구함
        e0 = p1 - p0;
        e1 = p2 - p0;

        // 두 벡터의 외적을 구함
        normal = Cross(e0, e1).GetNormalized();

        // 해당 인덱스의 노말값에 구해진 노말값을 구함
        // Terrain에서 해당 인덱스의 정점은 최소 1개, 최대 6개쯤 삼각형을 관여함
        // normal 벡터 값을 계속 누적해서 더하면 해당 면을 구성하는 정점들의 normal 벡터 값의 평균을 구할 수 있다.
        vertices[index0].normal += normal;
        vertices[index1].normal += normal;
        vertices[index2].normal += normal;
    }
}

void TerrainEditor::MakeTangentVector()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT> indices = mesh->GetIndices();

    UINT index0, index1, index2;
    Vector3 p0, p1, p2;
    Vector2 uv0, uv1, uv2;
    Vector3 e0, e1;
    Vector3 tangent;

    for (UINT i = 0; i < indices.size() / 3; i++)
    {
        // 정점 인덱스 가져옴
        index0 = indices[i * 3 + 0];
        index1 = indices[i * 3 + 1];
        index2 = indices[i * 3 + 2];

        // 각 정점의 포지션 가져옴
        p0 = vertices[index0].pos;
        p1 = vertices[index1].pos;
        p2 = vertices[index2].pos;

        // 각 정점의 uv값 가져옴
        uv0 = vertices[index0].uv;
        uv1 = vertices[index1].uv;
        uv2 = vertices[index2].uv;

        // 계산하기 편하게 원점으로 부터 떨어진 position을 구함
        e0 = p1 - p0;   // x
        e1 = p2 - p0;   // y

        // 0 -> 1번 정점으로 가는 비율 (x)
        float u1 = uv1.x - uv0.x;
        float v1 = uv1.y - uv0.y;

        // 0 -> 2번 정점으로 가는 비율 (y)
        float u2 = uv2.x - uv0.x;
        float v2 = uv2.y - uv0.y;

        float d = 1.0f / (u1 * v2 - v1 * u2);   // 두 벡터의 비율 계산
        tangent = d * (e0 * v2 - e1 * v1);      // 흠 탄젠트 ..?

        vertices[index0].tangent += tangent;
        vertices[index1].tangent += tangent;
        vertices[index2].tangent += tangent;
    }
}

void TerrainEditor::MakeComputeData()
{
    vector<VertexType> vertices = mesh->GetVertices();
    vector<UINT> indices = mesh->GetIndices();

    triangleSize = indices.size() / 3;

    inputs.resize(triangleSize);
    outputs.resize(triangleSize);

    // 정점 3개 입력
    for (UINT i = 0; i < triangleSize; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        inputs[i].v0 = vertices[index0].pos;
        inputs[i].v1 = vertices[index1].pos;
        inputs[i].v2 = vertices[index2].pos;
    }
}

void TerrainEditor::SaveHeightMap()
{
    UINT width = terrainSize.x+1;
    UINT height = terrainSize.y+1;

    if (ImGui::Button("SaveHeight"))
        DIALOG->OpenDialog("SaveHeight", "SaveHeight", ".png", ".");

    if (DIALOG->Display("SaveHeight"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            UINT size = width * height * 4;
            uint8_t* pixels = new uint8_t[size];

            vector<VertexType> vertices = mesh->GetVertices();

            for (UINT i = 0; i < size / 4; i++)
            {
                float y = vertices[i].pos.y;

                uint8_t height = ((y - MIN_HEIGHT) / (MAX_HEIGHT - MIN_HEIGHT)) * 255;

                pixels[i * 4 + 0] = height;
                pixels[i * 4 + 1] = height;
                pixels[i * 4 + 2] = height;
                pixels[i * 4 + 3] = 255;
            }

            // 이미지 정의
            Image image;
            image.width = width;
            image.height = height;
            image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
            image.rowPitch = width * 4;
            image.slicePitch = size;
            image.pixels = pixels;

            SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
                GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());

            delete[] pixels;
        }

        DIALOG->Close();
    }
}

void TerrainEditor::LoadHeightMap()
{
    if (ImGui::Button("LoadHeight"))
        DIALOG->OpenDialog("LoadHeight", "LoadHeight", ".png", ".");

    if (DIALOG->Display("LoadHeight"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            heightMap = Texture::AddTexture(ToWString(file));

            Resize();
        }

        DIALOG->Close();
    }
}

void TerrainEditor::SaveAlphaMap()
{
    UINT width = terrainSize.x + 1;
    UINT height = terrainSize.y + 1;

    if (ImGui::Button("SaveAlpha"))
        DIALOG->OpenDialog("SaveAlpha", "SaveAlpha", ".png", ".");

    if (DIALOG->Display("SaveAlpha"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            UINT size = width * height * 4;
            uint8_t* pixels = new uint8_t[size];

            vector<VertexType> vertices = mesh->GetVertices();

            for (UINT i = 0; i < size / 4; i++)
            {
                pixels[i * 4 + 0] = vertices[i].alpha[0] * 255;
                pixels[i * 4 + 1] = vertices[i].alpha[1] * 255;
                pixels[i * 4 + 2] = vertices[i].alpha[2] * 255;
                pixels[i * 4 + 3] = 255;
            }

            Image image;
            image.width = width;
            image.height = height;
            image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
            image.rowPitch = width * 4;
            image.slicePitch = size;
            image.pixels = pixels;

            SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
                GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());

            delete[] pixels;
        }

        DIALOG->Close();
    }
}

void TerrainEditor::LoadAlphaMap()
{
    if (ImGui::Button("LoadAlpha"))
        DIALOG->OpenDialog("LoadAlpha", "LoadAlpha", ".png", ".");

    if (DIALOG->Display("LoadAlpha"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Texture* alphaMap = Texture::AddTexture(ToWString(file));

            vector<Float4> pixels;
            alphaMap->ReadPixels(pixels);

            vector<VertexType>& vertices = mesh->GetVertices();

            for (UINT i = 0; i < vertices.size(); i++)
            {
                vertices[i].alpha[0] = pixels[i].z;
                vertices[i].alpha[1] = pixels[i].y;
                vertices[i].alpha[2] = pixels[i].x;
                vertices[i].alpha[3] = pixels[i].w;
            }

            mesh->UpdateVertex();
        }

        DIALOG->Close();
    }
}
