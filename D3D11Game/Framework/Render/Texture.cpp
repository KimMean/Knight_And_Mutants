#include "Framework.h"

unordered_map<wstring, Texture*> Texture::textures;

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file)
    : srv(srv), image(move(image)), file(file)
{
}

Texture::~Texture()
{
    if (!isReferenced)
        srv->Release();
}

void Texture::SetPixelShaderSlot(UINT slot)
{
    DC->PSSetShaderResources(slot, 1, &srv);
}

void Texture::ReadPixels(vector<Float4>& pixels)
{
    uint8_t* colors = image.GetPixels();
    UINT size = image.GetPixelsSize();

    pixels.resize(size / 4);

    float scale = 1.0f / 255.0f;

    for (UINT i = 0; i < pixels.size(); i++)
    {
        pixels[i].x = colors[i * 4 + 0] * scale;
        pixels[i].y = colors[i * 4 + 1] * scale;
        pixels[i].z = colors[i * 4 + 2] * scale;
        pixels[i].w = colors[i * 4 + 3] * scale;
    }
}

Texture* Texture::AddTexture(wstring file)
{
    wstring key = GetFileName(file);

    if (textures.count(key) > 0)
        return textures[key];

    CreateSRV(file);

    return textures[key];
}

Texture* Texture::AddTexture(wstring file, wstring saparator)
{
    //wstring fileName = GetFileName(file);
    wstring name = GetFileNameWithoutExtension(file);
    wstring extension = GetExtension(file);

    wstring key = name + saparator + L"." + extension;


    if (textures.count(key) > 0)
        return textures[key];

    CreateSRV(file, key);

    return textures[key];
}

Texture* Texture::AddTexture(wstring key, ID3D11ShaderResourceView* srv)
{
    if (textures.count(key) > 0)
        return textures[key];

    ScratchImage image;
    textures[key] = new Texture(srv, image, key);
    textures[key]->isReferenced = true;

    return textures[key];
}

void Texture::CreateSRV(wstring file, wstring key)
{
    if(!(key.length() > 0))
        key = GetFileName(file);
    wstring extension = GetExtension(file);

    HRESULT result;
    ScratchImage image;

    if (extension.compare(L"tga") == 0)
        result = LoadFromTGAFile(file.c_str(), nullptr, image);
    else if (extension.compare(L"dds") == 0)
        result = LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        result = LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image);

    assert(SUCCEEDED(result));

    ID3D11ShaderResourceView* srv;

    CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv);

    textures[key] = new Texture(srv, image, file);
    textures[key]->isReferenced = false;
}

void Texture::Delete()
{
    for (pair<wstring, Texture*> texture : textures)
        delete texture.second;
}
