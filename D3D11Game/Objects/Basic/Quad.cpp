#include "Framework.h"

Quad::Quad(Vector2 size) : size(size)
{
    gameObject = new GameObject();

    mesh = new Mesh<VertexType>();
    MakeMesh();
    mesh->CreateMesh();
    //Texture* texutre = Texture::AddTexture(L"Textures/Dirt.png");
    //SetTexture(L"Textures/Dirt.png");
    //gameObject->GetTransform()->Pos() -= (0, 0, 10);
}

Quad::~Quad()
{
    delete gameObject;
    delete mesh;
}

void Quad::Render()
{
    gameObject->SetRender();
    gameObject->transform->UpdateWorld();
    //DC->PSSetShaderResources(0, 1, &srv);

    mesh->Draw();
}

void Quad::SetTexture(wstring file)
{
    gameObject->material->SetDiffuseMap(file);
    //texture = Texture::AddTexture(file);
    //texture->SetPixelShaderSlot();
    //ID3D11SamplerState* state = nullptr;
    //D3D11_SAMPLER_DESC desc = {};
    //desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    //desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    //desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    //desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;;
    //desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    //desc.MinLOD = 0;
    //desc.MaxLOD = D3D11_FLOAT32_MAX;
    //
    //DEVICE->CreateSamplerState(&desc, &state);

    //HRESULT result;
    //ScratchImage image;
    //
    //// 확장자 확인
    //size_t index = file.find_last_of('.');
    //wstring extension = file.substr(index + 1);
    //
    //if (extension.compare(L"png") == 0)
    //    result = LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image);
    //
    //if (FAILED(result))
    //{
    //    // 파일 로드 실패
    //    return;
    //}
    //
    //// 리소스뷰 만들기
    //CreateShaderResourceView(
    //    DEVICE,
    //    image.GetImages(),
    //    image.GetImageCount(),
    //    image.GetMetadata(),
    //    &srv
    //);
    //
    //DC->PSSetShaderResources(0, 1, &srv);

    //DC->PSSetSamplers(0, 1, &state);

}

bool Quad::IsPointCollision(Vector3 pos)
{
    Vector2 halfSize = GetSize() * 0.5f;
    Vector3 position = gameObject->transform->GlobalPos();

    float x = pos.x - position.x;
    float y = pos.y - position.y;

    if (abs(x) < halfSize.x && abs(y) < halfSize.y)
        return true;

    return false;
}

void Quad::MakeMesh()
{
    Vector2 halfSize = size * 0.5f;

    vector<VertexType>& vertices = mesh->GetVertices();

    //vertices.emplace_back(left, bottom, 0.0f, 1, 1, 1);
    //vertices.emplace_back(left, top, 0.0f, 1, 1, 1);
    //vertices.emplace_back(right, top, 0.0f, 1, 1, 1);
    //vertices.emplace_back(right, bottom, 0.0f, 1, 1, 1);

    vertices.emplace_back(-halfSize.x, -halfSize.y, 0.0f, 0, 1);
    vertices.emplace_back(-halfSize.x, +halfSize.y, 0.0f, 0, 0);
    vertices.emplace_back(+halfSize.x, +halfSize.y, 0.0f, 1, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, 0.0f, 1, 1);

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        0, 1, 2, 2, 3, 0
    };
}
