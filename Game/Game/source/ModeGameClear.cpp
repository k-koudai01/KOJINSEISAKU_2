#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGameClear.h"
#include "ModeGame.h"
#include "ModeTitle.h"
#include "BulletManager.h"

namespace
{
	// 描画設定
	constexpr float LOGO_FONT_SIZE = 64.0f;
	constexpr float ITEM_FONT_SIZE = 28.0f;

	constexpr float LOGO_Y_RATIO  = 0.3f;
	constexpr float ITEM_Y_RATIO  = 0.75f;
	constexpr float ITEM_OFFSET_X = 180.0f;

	// 色
	const unsigned int COLOR_LOGO     = GetColor(255, 215,   0);
	const unsigned int COLOR_NORMAL   = GetColor(180, 180, 180);
	const unsigned int COLOR_SELECTED = GetColor(255, 255, 255);

	// メニュー項目テーブル
	struct MenuItemData
	{
		ModeGameClear::Item item;
		const char* label;
		float offsetXRatio;
	};

	const MenuItemData MENU_ITEMS[] =
	{
		{ ModeGameClear::Item::Title, "TITLE", -1.0f }, // 左側
		{ ModeGameClear::Item::Retry, "RETRY",  1.0f }, // 右側
	}; 
}

bool ModeGameClear::Initialize()
{
	if(!base::Initialize()) return false;
	_selectedItem  = Item::Title;
	_inputCooldown = 0.5f;
	return true;
}

bool ModeGameClear::Terminate()
{
	base::Terminate();
	return true;
}

bool ModeGameClear::Process()
{
	base::Process();

	ModeServer::GetInstance()->SkipProcessUnderLayer();

	if(_inputCooldown > 0.0f)
	{
		_inputCooldown -= 1.0f / 60.0f;
		return true;
	}

	int trg = ApplicationMain::GetInstance()->GetTrg();

	if(trg & PAD_INPUT_LEFT)  { SelectNext(); }
	if(trg & PAD_INPUT_RIGHT) { SelectPrev(); }

	if(trg & PAD_INPUT_1)
	{
		ModeBase* game = ModeServer::GetInstance()->Get("modeGame");
		if(game)
		{
			ModeServer::GetInstance()->Del(game);
		}

		if(_selectedItem == Item::Title)
		{
			ModeBase* title = ModeServer::GetInstance()->Get("modeTitle");
			if(!title)
			{
				ModeServer::GetInstance()->Add(new ModeTitle(), 1, "modeTitle");
			}
		}
		else if(_selectedItem == Item::Retry)
		{
			ModeServer::GetInstance()->Add(new ModeGame(), 1, "modeGame");
		}
		ModeServer::GetInstance()->Del(this);
	}

	return true;
}

bool ModeGameClear::Render()
{
	base::Render();

	int sw = ScreenW();
	int sh = ScreenH();

	// 背景を半透明の黒で覆う
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, sw, sh, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 描画
	DrawGameClearLogo();
	DrawMenuItems();
	return true;
}

void ModeGameClear::SelectNext()
{
	int current = static_cast<int>(_selectedItem);
	current = (current + 1) % static_cast<int>(Item::Max);
	_selectedItem = static_cast<Item>(current);
}

void ModeGameClear::SelectPrev()
{
	int current = static_cast<int>(_selectedItem);
	current = (current - 1 + static_cast<int>(Item::Max)) % static_cast<int>(Item::Max);
	_selectedItem = static_cast<Item>(current);
}

void ModeGameClear::DrawGameClearLogo()
{
	float scale = GetScale();
	int sw = ScreenW();
	int sh = ScreenH();

	SetFontSize(static_cast<int>(LOGO_FONT_SIZE * scale));

	const char* logo = "GAME CLEAR";
	int logoW = GetDrawStringWidth(logo, strlen(logo), 1);
	int logoX = static_cast<int>(sw / 2.0f - logoW / 2.0f);
	int logoY = static_cast<int>(sh * LOGO_Y_RATIO);
	
	DrawString(logoX, logoY, logo, COLOR_LOGO);
}

void ModeGameClear::DrawMenuItems()
{
	float scale = GetScale();
	int sw = ScreenW();
	int sh = ScreenH();

	SetFontSize(static_cast<int>(ITEM_FONT_SIZE * scale));

	int centerX = sw / 2;
	int itemY = static_cast<int>(sh * ITEM_Y_RATIO);
	float offsetX = ITEM_OFFSET_X * scale;

	for(int i = 0; i < static_cast<int>(Item::Max); ++i)
	{
		const auto& itemData = MENU_ITEMS[i];
		bool isCurrent       = (_selectedItem == itemData.item);
		unsigned int color   = isCurrent ? COLOR_SELECTED : COLOR_NORMAL;

		// 画面中央から左右に振る位置を計算
		int posX = centerX + static_cast<int>(itemData.offsetXRatio * offsetX);

		int textW = GetDrawStringWidth(itemData.label, static_cast<int>(strlen(itemData.label)));
		int textX = posX - textW / 2;

		if(isCurrent)
		{
			int cursorX = textX - static_cast<int>(25.0f * scale);
			DrawString(cursorX, itemY, ">", color);
		}

		DrawString(textX, itemY, itemData.label, color);
	}
}