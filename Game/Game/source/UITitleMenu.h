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

	// 描画の細分化
	void DrawTitleLogo(float sw, float sh, float scale);
	int  DrawMenuPanel(float sw, float sh, float scale, float scaleY);
	// メニュー項目の描画
	void DrawMenuItems(float sw, int panelY, float scale, float scaleY);
	void DrawMenuItem(Item item, const char* label, int centerX, int yPos, float scale);
private:
	Item  _selectedItem = Item::Start;
	bool  _isDecided    = false;
	float _decideTimer  = 0.0f;
};

