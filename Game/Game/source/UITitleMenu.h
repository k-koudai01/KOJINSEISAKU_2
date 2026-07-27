#pragma once
#include "UIBase.h"

class UITitleMenu : public UIBase
{
	typedef UIBase base;

public:
	enum class Item {Start = 0, Exit, Max };

	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;
	
	Item GetSelectedItem() const { return _selectedItem; }
	bool IsDecided() const { return _isDecided; }
private:
	void DrawMenuItem(Item item, const char* label, int yPos);

	Item  _selectedItem = Item::Start;
	bool  _isDecided    = false;
	float _decideTimer  = 0.0f;
};

