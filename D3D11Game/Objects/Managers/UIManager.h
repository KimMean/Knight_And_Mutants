#pragma once

class UIManager : public Singleton<UIManager>
{
private :
	friend class Singleton;

	UIManager();
	~UIManager();

public :
	void Update();
	void PostRender();

public:
	Inventory* GetInventory() { return inventory; }
	bool& InventoryIsShow() { return inventoryIsShow; }

	bool& IsInteraction() { return isInteraction; }

	TextBox* GetTextBox() { return textBox; }
	bool& TextBoxIsShow() { return textBoxIsShow; }

	ProgressBar* GetHPBar() { return hpBar; }

private :
	Inventory* inventory;
	bool inventoryIsShow = false;

	Interaction* interaction;
	bool isInteraction = false;

	TextBox* textBox;
	bool textBoxIsShow = false;

	ProgressBar* hpBar;


};