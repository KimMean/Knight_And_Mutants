#pragma once

class Item : public Quad
{
public :
	Item(Vector2 size);
	~Item();

	void Update();
	void Render();

	string GetName() { return name; }
	void SetName(string name);
	void SetTexture(wstring textureFile);
	Slot* GetSlot() { return slot; }
	void SetSlot(Slot* slot);

	void AddCount() { count++; }
	void DeleteCount() { count -= 10; }
	void UseItem() { count--; }

	//bool IsPointCollision(Vector3 mousePos);

private :
	string name;
	UINT count = 0;
	Slot* slot = nullptr;	
};