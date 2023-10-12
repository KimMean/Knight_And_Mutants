#pragma once

class Inventory
{
public :
	Inventory();
	~Inventory();

	void Update();
	void PostRender();

public :
	void AddItem(string name);
	void DeleteItem(string name);
	void UseItem(string name);

private :
	void CreateInventory();
	void CreateSlots();
	void CreateQuickSlots();
public :
	void CreateItems(wstring TextureFile);

private:
	void TakeItem();
	void ItemDragAndDrop();

private :
	Quad* panel;
	vector<Slot*> slots;
	vector<Slot*> quickSlots;

	vector<Item*> items;
	Item* targetItem = nullptr;

};