#pragma once

class Texture
{
private :
	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file);
	~Texture();

public :
	void SetPixelShaderSlot(UINT slot = 0);

	Vector2 GetSize()
	{
		return Vector2(image.GetMetadata().width, image.GetMetadata().height);
	}
	void ReadPixels(vector<Float4>& pixels);

	wstring GetFile() { return file; }
	wstring GetKey() { return GetFileName(file); }
	ID3D11ShaderResourceView* GetSRV() { return srv; }

	static Texture* AddTexture(wstring file = L"Textures/Color/White.png");
	static Texture* AddTexture(wstring file, wstring saparator);
	static Texture* AddTexture(wstring key, ID3D11ShaderResourceView* srv);
	static void CreateSRV(wstring file, wstring key = L"");

	static void Delete();


private :
	wstring file;

	ScratchImage image;
	ID3D11ShaderResourceView* srv = nullptr;

	bool isReferenced = false;

	static unordered_map<wstring, Texture*> textures;
};