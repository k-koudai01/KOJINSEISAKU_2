#include "UITitleMenu.h"
#include "UIRender.h"

namespace
{
	// 開発時の基準解像度
	constexpr int SCREEN_WIDTH = 1280;
	constexpr int SCREEN_HEIGHT = 720;

	// UI設定用の定数群
	constexpr float TITLE_FONT_SIZE = 48.0f;
	constexpr float ITEM_FONT_SIZE  = 24.0f;

	constexpr float PANEL_WIDTH     = 300.0f;
	constexpr float PANEL_HEIGHT    = 120.0f;
	constexpr float PANEL_Y_OFFSET  = 20.0f;   // 画面中央からのYオフセット
										  
	constexpr float CURSOR_X_OFFSET = 100.0;  // 画面中央からのカーソル位置
	constexpr float TEXT_X_OFFSET   = 70.0;   // 画面中央からのテキスト位置
	constexpr float ITEM_SPACING    = 45.0;   // 項目同士の間隔

	constexpr int BLINK_INTERVAL_MS = 80; // 決定時の点滅間隔

	// 色の定数
	const unsigned int COLOR_TITLE    = GetColor(0, 0, 0);
	const unsigned int COLOR_NORMAL   = GetColor(180, 180, 180);
	const unsigned int COLOR_SELECTED = GetColor(255, 255, 0);
	const unsigned int COLOR_DECIDED  = GetColor(255, 100, 100);
	const unsigned int COLOR_PANEL_BG = GetColor(0, 0, 0);

	// メニュー項目のデータテーブル
	struct MenuItemData
	{
		UITitleMenu::Item item;
		const char* label;
	};

	const MenuItemData MENU_ITEMS[] =
	{
		{ UITitleMenu::Item::Start, "GAME START" },
		{ UITitleMenu::Item::Exit,  "GAME EXIT"  },
	};
}


bool UITitleMenu::Initialize()
{
	if(!base::Initialize()) { return false; }
	_selectedItem = Item::Start;
	_isDecided    = false;
	_decideTimer  = 0.0f;
	return true;
}

bool UITitleMenu::Terminate()
{
	base::Terminate();
	return true;
}

bool UITitleMenu::Process()
{
	base::Process();

	if(_isDecided)
	{
		_decideTimer += 1.0f / 60.0f;
		return true;
	}

	int trg = ApplicationBase::GetInstance()->GetTrg();

	// 上下入力
	if(trg & PAD_INPUT_UP)
	{
		SelectPrev();
	}
	if(trg & PAD_INPUT_DOWN)
	{
		SelectNext();
	}

	// 決定入力
	if(trg & PAD_INPUT_1)
	{
		_isDecided = true;
	}
	return true;

}

bool UITitleMenu::Render()
{
	base::Render();

	float sw = static_cast<float>(ApplicationBase::GetInstance()->DispSizeW());
	float sh = static_cast<float>(ApplicationBase::GetInstance()->DispSizeH());

	// 解像度に応じたスケーリングを計算
	float scaleX = sw / SCREEN_WIDTH;
	float scaleY = sh / SCREEN_HEIGHT;
	// 縦横の歪みを防ぐため、小さい方のスケールを使う
	float scale = (scaleX < scaleY) ? scaleX : scaleY;

	// 描画処理
	DrawTitleLogo(sw, sh, scale);
	int panelY = DrawMenuPanel(sw, sh, scale, scaleY);
	DrawMenuItems(sw, panelY, scale, scaleY);

	return true;
}

void UITitleMenu::SelectNext()
{
	int current   = static_cast<int>(_selectedItem);
	current       = (current + 1) % static_cast<int>(Item::Max);
	_selectedItem = static_cast<Item>(current);
}

void UITitleMenu::SelectPrev()
{
	int current = static_cast<int>(_selectedItem);
	current		= (current - 1 + static_cast<int>(Item::Max)) % static_cast<int>(Item::Max);
	_selectedItem = static_cast<Item>(current);
}

void UITitleMenu::DrawTitleLogo(float sw, float sh, float scale)
{
	// タイトルロゴ
	int titleFontSize = static_cast<int>(TITLE_FONT_SIZE * scale);
	SetFontSize(titleFontSize);

	const char* title = "MY ACTION GAME";
	int titleW = GetDrawStringWidth(title, static_cast<int>(strlen(title)));
	int titleX = static_cast<int>(sw / 2 - titleW / 2.0);
	int titleY = static_cast<int>(sh / 4);
	DrawString(titleX, titleY, title, COLOR_TITLE);
}

int UITitleMenu::DrawMenuPanel(float sw, float sh, float scale, float scaleY)
{
	// メニュー枠パネル
	int panelW = static_cast<int>(PANEL_WIDTH * scale);
	int panelH = static_cast<int>(PANEL_HEIGHT * scale);
	int panelX = static_cast<int>(sw / 2.0f - panelW / 2.0f);
	int panelY = static_cast<int>(sh / 2.0f + PANEL_Y_OFFSET * scaleY);

	UIRender::DrawPanel(panelX, panelY, panelW, panelH, COLOR_PANEL_BG, COLOR_TITLE);
	return panelY;
}

void UITitleMenu::DrawMenuItem(Item item, const char* label, int centerX, int yPos, float scale) 
{
	bool isCurrent = (_selectedItem == item);

	int itemFontSize = static_cast<int>(ITEM_FONT_SIZE * scale);
	SetFontSize(itemFontSize);

	unsigned int color = COLOR_NORMAL;

	if(isCurrent)
	{
		bool isBlink = _isDecided && (GetNowCount() / BLINK_INTERVAL_MS % 2 == 0);
		color = isBlink ? COLOR_DECIDED : COLOR_SELECTED;

		// スケールに応じたカーソルの座標
		int cursorX = centerX - static_cast<int>(CURSOR_X_OFFSET * scale);
		DrawString(cursorX, yPos, ">", color);
	}
	int textX = centerX - static_cast<int>(TEXT_X_OFFSET * scale);
	DrawString(textX, yPos, label, color);
}

void UITitleMenu::DrawMenuItems(float sw, int panelY, float scale, float scaleY)
{
	int centerX = static_cast<int>(sw / 2.0f);
	int startY  = panelY + static_cast<int>(20.0f * scaleY);
	int spacing = static_cast<int>(ITEM_SPACING * scaleY);

	for(int i = 0; i < static_cast<int>(Item::Max); ++i)
	{
		int itemY = startY + (i * spacing);
		DrawMenuItem(MENU_ITEMS[i].item, MENU_ITEMS[i].label, centerX, itemY, scale);
	}
}