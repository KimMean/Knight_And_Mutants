#include "Framework.h"

Inventory::Inventory()
{
	CreateInventory();
	CreateSlots();
	CreateQuickSlots();
	CreateItems(L"textures/Icons/Sword.png");
	//items[0]->GetSlot()->isEmpty = true;
	items[0]->SetSlot(quickSlots[0]);
	//CreateItems(L"textures/Icons/Axe.png");
	//CreateItems(L"textures/Icons/IceAxe.png");
	//CreateItems(L"textures/Icons/Hammer.png");
	//CreateItems(L"textures/Icons/Log.png");
	//CreateItems(L"textures/Icons/Stone.png");
	CreateItems(L"textures/Icons/Apple.png");
	//CreateItems(L"textures/Color/Red.png");
}

Inventory::~Inventory()
{
	delete panel;
	for (Slot* slot : slots)
		delete slot;
	for (Slot* slot : quickSlots)
		delete slot;
	for (Item* item : items)
		delete item;
}

void Inventory::Update()
{
	if (targetItem)
		targetItem->gameObject->transform->Pos() = screenMousePos;

	if(KEY_DOWN(MK_LBUTTON))
	{
		if (targetItem)
		{
			ItemDragAndDrop();
		}
		else
		{
			TakeItem();
		}
	}
}

void Inventory::PostRender()
{
	for (Slot* slot : quickSlots)
		slot->Render();
	if (UIManager::Get()->InventoryIsShow())
	{
		panel->Render();
		for (Slot* slot : slots)
			slot->Render();
		
		for (Item* item : items)
			item->Render();
	}
	else
	{
		for (Item* item : items)
		{
			for (Slot* slot : quickSlots)
			{
				if (item->GetSlot() == slot)
					item->Render();
			}
		}
	}
}

void Inventory::AddItem(string name)
{
	for (Item* item : items)
	{
		if (item->GetName() == name)
		{
			item->AddCount();
			return;
		}
	}
	CreateItems(L"textures/Icons/" + ToWString(name) + L".png");

}

void Inventory::DeleteItem(string name)
{
	for (Item* item : items)
	{
		if (item->GetName() == name)
		{
			item->DeleteCount();
			return;
		}
	}
	CreateItems(L"textures/Icons/" + ToWString(name) + L".png");
}

void Inventory::UseItem(string name)
{
	for (Item* item : items)
	{
		if (item->GetName() == name)
		{
			item->UseItem();
			return;
		}
	}
}

void Inventory::CreateInventory()
{
	panel = new Quad(Vector2(500.0f, 500.0f));
	panel->gameObject->material->SetShaderFile(L"Shaders/Basic/UI.hlsl");
	panel->gameObject->material->SetDiffuseMap(L"Textures/Icons/Inventory.png");
	panel->gameObject->transform->Pos() = { CENTER_X, CENTER_Y, 0.0f };
	panel->gameObject->transform->UpdateWorld();
}

void Inventory::CreateSlots()
{
	Vector2 slotSize = { 50.0f, 50.0f };
	Vector2 startPos = { -180.0f, 180.0f };
	Vector2 padding = { 10.0f, 10.0f };
	for (UINT y = 0; y < 7; y++)
	{
		for (UINT x = 0; x < 7; x++)
		{
			float posX = startPos.x + slotSize.x * x + padding.x * x;
			float posY = startPos.y - slotSize.y * y - padding.y * y;
			Slot* quad = new Slot(slotSize);
			quad->gameObject->transform->SetParent(panel->gameObject->transform);
			quad->gameObject->transform->Pos() = { posX, posY, 0.0f };
			quad->gameObject->transform->UpdateWorld();
			slots.push_back(quad);
		}
	}
}

void Inventory::CreateQuickSlots()
{
	Vector2 slotSize = { 50.0f, 50.0f };
	Vector2 startPos = { CENTER_X - 180.0f, 50.0f };
	Vector2 padding = { 10.0f, 10.0f };

	for (UINT i = 0; i < 7; i++)
	{
		float posX = startPos.x + slotSize.x * i + padding.x * i;
		Slot* quad = new Slot(slotSize);
		quad->gameObject->transform->Pos() = { posX, startPos.y, 0.0f };
		quad->gameObject->transform->UpdateWorld();
		quickSlots.push_back(quad);
	}

}

void Inventory::CreateItems(wstring TextureFile)
{
	Vector2 slotSize = { 50.0f, 50.0f };
	Item* item = new Item(slotSize);
	//quad->gameObject->transform->Pos() = { 50.0f, 50.0f, 0.0f };
	item->SetTexture(TextureFile);

	for (Slot* slot : slots)
	{
		if (slot->isEmpty)
		{
			item->SetSlot(slot);
			break;
		}
	}

	items.push_back(item);
	//slots.push_back(quad);

}

void Inventory::TakeItem()
{
	for (Item* item : items)
	{
		if (item->IsPointCollision(screenMousePos))
		{
			targetItem = item;
			item->GetSlot()->isEmpty = true;
			Audio::Get()->Play("Slot");
			break;
		}
	}
}

void Inventory::ItemDragAndDrop()
{
	bool isChange = false;
	for (Item* item : items)
	{
		if (item->IsPointCollision(screenMousePos))
		{
			if (item == targetItem)
				continue;

			targetItem->SetSlot(item->GetSlot());
			item->SetSlot(nullptr);
			targetItem = item;
			isChange = true;
			Audio::Get()->Play("Slot");
			break;
		}
	}
	if (!isChange)
	{
		for (Slot* slot : slots)
		{
			if (slot->IsPointCollision(screenMousePos))
			{
				targetItem->SetSlot(slot);
				targetItem = nullptr;
				slot->isEmpty = false;
				isChange = true;
				Audio::Get()->Play("Slot");
				break;
			}
		}
	}

	if (!isChange)
	{
		for (Slot* slot : quickSlots)
		{
			if (slot->IsPointCollision(screenMousePos))
			{
				targetItem->SetSlot(slot);
				targetItem = nullptr;
				slot->isEmpty = false;
				isChange = true;
				Audio::Get()->Play("Slot");
			}
		}
	}
}
