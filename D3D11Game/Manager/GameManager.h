#pragma once

const float GRAVITY = 9.8f;

class GameManager
{
public :
	GameManager();
	~GameManager();

	void Update();
	void Render();

private :
	void Create();
	void Delete();

private :

	Scene* scene;
};