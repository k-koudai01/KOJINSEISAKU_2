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
	bool IsDecided() const       { return _isDecided;    }
private:
	// メニュー操作の内部関数
	void SelectNext();
	void SelectPrev();

	// メニュー項目の描画
	void DrawMenuItem(Item item, const char* label, int centerX, int yPos, float scale);

private:
	Item  _selectedItem = Item::Start;
	bool  _isDecided    = false;
	float _decideTimer  = 0.0f;
};

