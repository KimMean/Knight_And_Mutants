#pragma once

class Interaction : public Quad
{
public :
	Interaction(Vector2 size);
	~Interaction();

	void Render();

	Quad* GetIcon();
private :
	Quad* icon;
};