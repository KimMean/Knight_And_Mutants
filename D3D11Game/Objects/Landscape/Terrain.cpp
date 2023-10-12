#include "Framework.h"

Terrain::Terrain()
{
    LoadTerrain();

    gameObject = new GameObject();
    gameObject->material->SetShaderFile(L"Shaders/Landscape/Terrain.hlsl");
    gameObject->material->SetDiffuseMap(sandTexture);

}

Terrain::~Terrain()
{
    delete gameObject;
    delete mesh;
}

void Terrain::Update()
{
}

void Terrain::Render()
{
    if(depthTexture)
        depthTexture->SetPixelShaderSlot(9);
    alphaMap->SetPixelShaderSlot(10);
    sandTexture->SetPixelShaderSlot(11);
    grassTexture->SetPixelShaderSlot(12);
    soilTexture->SetPixelShaderSlot(13);

    gameObject->SetRender();
    //Environment::Get()->Set
    mesh->Draw();
}

void Terrain::SetDepthTexture(Texture* texture)
{
    depthTexture = texture;
}

float Terrain::GetHeight(const Vector3& pos, Vector3* normal)
{
    int x = (int)pos.x;
    int z = (int)(height - pos.z);
    //int z = (int)(height - pos.z - 1);

    if (x < 0 || x >= width) return 0.0f;
    if (z < 0 || z >= height) return 0.0f;

    // �ش� ��ġ�� �ִ� ���� �ε��� 4��
    UINT index[4];
    index[0] = (width + 1) * z + x;
    index[1] = (width + 1) * z + x + 1;
    index[2] = (width + 1)* (z + 1) + x;
    index[3] = (width + 1) * (z + 1) + x + 1;

    vector<VertexType> vertices = mesh->GetVertices();

    // ���� ��ġ
    Vector3 p[4];
    for (UINT i = 0; i < 4; i++)
        p[i] = vertices[index[i]].pos;  // 0,1,2,2,1,3
    // �ش� ��ġ�� uv��
    float u = (pos.x - p[0].x);
    float v = (p[0].z - pos.z);

    Vector3 result;
    // �밢������ �߶��� ��
    if (u + v <= 1.0)   // ���� ���
    {
        // ���� ��ġ ���
        result = ((p[1] - p[0]) * u + (p[2] - p[0]) * v) + p[0];

        if (normal)
        {
            (*normal) = GetNormalFromPolygon(p[0], p[1], p[2]);
        }
        return result.y;
    }
    else  // ������ �ϴ�
    {
        //u = 1.0f - u;
        v = 1.0f - v;

        result = ((p[3] - p[2]) * u + (p[1] - p[3]) * v) + p[2];

        if (normal)
        {
            (*normal) = GetNormalFromPolygon(p[2], p[1], p[3]);
        }
        return result.y;
    }
}

Vector3 Terrain::GetPosition(const Vector3& pos)
{
    int x = (int)pos.x;
    int z = (int)(height - pos.z);
    //int z = (int)(height - pos.z - 1);

    if (x < 0 || x >= width - 1) return 0.0f;
    if (z < 0 || z >= height - 1) return 0.0f;

    // �ش� ��ġ�� �ִ� ���� �ε��� 4��
    UINT index;
    index = (width+1) * z + x;

    vector<VertexType> vertices = mesh->GetVertices();
    
    return vertices[index].pos;
}


void Terrain::LoadTerrain()
{
    heightMap = Texture::AddTexture(L"Textures/Maps/Map_H64.png");
    alphaMap = Texture::AddTexture(L"Textures/Maps/Map_A64.png");
    // BlendTexture
    sandTexture = Texture::AddTexture(L"Textures/Landscape/sand.png");
    grassTexture = Texture::AddTexture(L"Textures/Landscape/grassy.png");
    soilTexture = Texture::AddTexture(L"Textures/Landscape/soil.png");

    mesh = new Mesh<VertexType>();
    MakeMesh();
    MakeNormalVector();
    MakeTangentVector();
    mesh->CreateMesh();
}

void Terrain::MakeMesh()
{
    // ĭ�� ���߱� ���� + 1
    width = (UINT)heightMap->GetSize().x - 1;
    height = (UINT)heightMap->GetSize().y - 1;
    // Pixels
    vector<Float4> pixels;
    heightMap->ReadPixels(pixels);
    //Vertices
    vector<VertexType>& vertices = mesh->GetVertices();
    vertices.reserve(width * height);

    // ���� ���������� uv���� ���缭 ������ ����
    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            // uv�� ���� �����ʹϱ�
            VertexType vertex;
            vertex.pos = { (float)x, 0.0f, (float)(height - z) };
            vertex.uv.x = x / (float)((width) * 0.1f);
            vertex.uv.y = z / (float)((height) * 0.1f);
            
            UINT index = (width + 1) * z + x;
            vertex.pos.y = pixels[index].x * (MAX_HEIGHT - MIN_HEIGHT) + MIN_HEIGHT;

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

void Terrain::MakeNormalVector()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT> indices = mesh->GetIndices();

    UINT index0, index1, index2;
    Vector3 p0, p1, p2;
    Vector3 e0, e1;  
    Vector3 normal;

    for (UINT i = 0; i < indices.size() / 3; i++)
    {
        // 3���� ���� �ε����� ����
        index0 = indices[i * 3 + 0];
        index1 = indices[i * 3 + 1];
        index2 = indices[i * 3 + 2];

        // 3�� ������ ��ġ�� �޾ƿ�
        p0 = vertices[index0].pos;
        p1 = vertices[index1].pos;
        p2 = vertices[index2].pos;

        // ����ϱ� ���ϰ� �������� ���� ������ position�� ����
        e0 = p1 - p0;
        e1 = p2 - p0;

        // �� ������ ������ ����
        normal = Cross(e0, e1).GetNormalized();

        // �ش� �ε����� �븻���� ������ �븻���� ����
        // Terrain���� �ش� �ε����� ������ �ּ� 1��, �ִ� 6���� �ﰢ���� ������
        // normal ���� ���� ��� �����ؼ� ���ϸ� �ش� ���� �����ϴ� �������� normal ���� ���� ����� ���� �� �ִ�.
        vertices[index0].normal += normal;
        vertices[index1].normal += normal;
        vertices[index2].normal += normal;
    }
}

void Terrain::MakeTangentVector()
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
        // ���� �ε��� ������
        index0 = indices[i * 3 + 0];
        index1 = indices[i * 3 + 1];
        index2 = indices[i * 3 + 2];

        // �� ������ ������ ������
        p0 = vertices[index0].pos;
        p1 = vertices[index1].pos;
        p2 = vertices[index2].pos;

        // �� ������ uv�� ������
        uv0 = vertices[index0].uv;
        uv1 = vertices[index1].uv;
        uv2 = vertices[index2].uv;

        // ����ϱ� ���ϰ� �������� ���� ������ position�� ����
        e0 = p1 - p0;   // x
        e1 = p2 - p0;   // y

        // 0 -> 1�� �������� ���� ���� (x)
        float u1 = uv1.x - uv0.x;
        float v1 = uv1.y - uv0.y;

        // 0 -> 2�� �������� ���� ���� (y)
        float u2 = uv2.x - uv0.x;
        float v2 = uv2.y - uv0.y;

        float d = 1.0f / (u1 * v2 - v1 * u2);   // �� ������ ���� ���
        tangent = d * (e0 * v2 - e1 * v1);      // �� ź��Ʈ ..?

        vertices[index0].tangent += tangent;
        vertices[index1].tangent += tangent;
        vertices[index2].tangent += tangent;
    }
}
