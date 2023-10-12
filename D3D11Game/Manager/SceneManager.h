#pragma once

class SceneManager : public Singleton<SceneManager>
{
private :
	friend class Singleton;

	SceneManager();
	~SceneManager();

public :
	void Update();

	void PreRender();
	void Render();
	void PostRender();
	void GUIRender();

	void Create(string key, Scene* scene);

	Scene* Add(string key);
	void Remove(string key);
private:
	void AddScene();
	void RemoveScene();

private:
	map<string, Scene*> scenes;

	list<Scene*> curScenes;

	string addScene, removeScene;
};