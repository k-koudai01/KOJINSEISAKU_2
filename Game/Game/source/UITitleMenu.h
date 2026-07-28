#pragma once
#include "UIBase.h"

class UITitleMenu : public UIBase
{
	typedef UIBase base;

public:
	// メニュー項目
	enum class Item {Start = 0, Exit, Max };

	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;
	
	// ゲッター
	Item GetSelectedItem() const { return _selectedItem; }
	bool IsDecided()       const { return _isDecided; }

private:
	// メニュー操作の内部関数
	void SelectNext();
	void SelectPrev();

	// 描画処理の細分化
	void DrawTitleLogo();
	int  DrawMenuPanel();
	void DrawMenuItems(int panelY);
	void DrawMenuItem(Item item, const char* label, int centerX, int yPos);

private:
	Item  _selectedItem = Item::Start; // 現在選択中の項目
	bool  _isDecided    = false;       // 決定フラグ
	float _decideTimer  = 0.0f;		   // 決定後演出用のタイマー
};

