#pragma once

class SkyBox
{
public :
	SkyBox();
	~SkyBox();

	void SetSkyBoxTexture(wstring textureFile);

	void Render();

private :
	Sphere* skyBox;
	Texture* cubeMap;

};