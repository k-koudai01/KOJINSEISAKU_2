#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ApplicationGlobal.h"
#include "ModeMenu.h"

namespace
{
	constexpr int MENU_X          = 128;
	constexpr int MENU_Y          = 128;
	constexpr int FONT_SIZE       = 32;
	constexpr int FONT_PITCH      = FONT_SIZE + 8;
	constexpr int PADDING	      = 16;
	constexpr int CURSOR_X_OFFSET = 64;
}

bool ModeMenu::Initialize()
{
	base::Initialize();
	_curPos = 0;
	_curAnimCnt = 0;
	return true;
}

bool ModeMenu::Terminate()
{
	base::Terminate();
	Clear();
	return true;
}

bool ModeMenu::Process()
{
	base::Process();
	
	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	UpdateInput();
	UpdateCursorAnim();

	return true;
}

bool ModeMenu::Render()
{
	base::Render();

	if (!HasItems())
	{
		return true;
	}

	int w = 0, h = 0;
	MeasureMenuSize(w, h);

	DrawMenuBackground(w, h);

	const int startY = PADDING / 2;
	DrawMenuItems(startY);
	DrawCursor(startY);

	return true;
}

void ModeMenu::Clear()
{
	// 登録したitemを全部削除する
	for (auto ite = _vItems.begin(); ite != _vItems.end();)
	{
		delete (*ite);
		ite = _vItems.erase(ite);
	}
}

void ModeMenu::Add(MenuItemBase* itemBase)
{
	_vItems.push_back(itemBase);
}

void ModeMenu::UpdateInput()
{
	const int trg = ApplicationMain::GetInstance()->GetTrg();

	const bool close = UpdateClose(trg);
	if(close)
	{
		ModeServer::GetInstance()->Del(this);
		return;
	}

	if(!HasItems())
	{
		_curPos = 0;
		return;
	}

	UpdateCursor(trg);

	const bool selected = UpdateSelect(trg);
	if(selected)
	{
		ModeServer::GetInstance()->Del(this);
	}
}

void ModeMenu::UpdateCursor(int trg)
{
	if(trg & PAD_INPUT_UP)   { _curPos--; _curAnimCnt = 0; }
	if(trg & PAD_INPUT_DOWN) { _curPos++; _curAnimCnt = 0; }

	// カーソル位置を上下ループ
	const int itemNum = GetItemCount();
	_curPos = (_curPos + itemNum) % itemNum;
}

bool ModeMenu::UpdateSelect(int trg)
{
	if (trg & PAD_INPUT_1)
	{
		const int ret = _vItems[_curPos]->Selected();
		return ret == 1;
	}
	return false;
}

bool ModeMenu::UpdateClose(int trg)
{
	if(trg & PAD_INPUT_9)
	{
		return true;
	}
	return false;
}

void ModeMenu::UpdateCursorAnim()
{
	_curAnimCnt++;
}

void ModeMenu::MeasureMenuSize(int& outW, int& outH) const
{
	int w = 0, h = 0;
	SetFontSize(FONT_SIZE);

	for (const auto* item : _vItems)
	{
		const int itemW = GetDrawStringWidth(item->_text.c_str(), static_cast<int>(item->_text.size()));
		if (w < itemW)
		{
			w = itemW;
		}
		h += FONT_PITCH;
	}

	// カーソル, 枠分のサイズ拡張
	w += CURSOR_X_OFFSET + PADDING;
	h += PADDING;

	outW = w;
	outH = h;
}

void ModeMenu::DrawMenuBackground(int w, int h) const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(MENU_X, MENU_Y, MENU_X + w, MENU_Y + h, GetColor(0, 0, 255), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(MENU_X, MENU_Y, MENU_X + w, MENU_Y + h, GetColor(0, 0, 255), FALSE);
}

void ModeMenu::DrawMenuItems(int startY) const
{
	int offsetY = 0;
	for (const auto* item : _vItems)
	{
		DrawString(MENU_X + CURSOR_X_OFFSET, MENU_Y + startY + offsetY, item->_text.c_str(), GetColor(255, 0, 0));
		offsetY += FONT_PITCH;
	}
}

void ModeMenu::DrawCursor(int startY) const
{
	DrawGraph
	(
		MENU_X + 4 + ((_curAnimCnt / 6) % 4) * 4,
		MENU_Y + startY + FONT_PITCH * _curPos,
		gGlobal._cgCursor,
		TRUE
	);
}

bool ModeMenu::HasItems() const
{
	return !_vItems.empty();
}

int ModeMenu::GetItemCount() const
{
	return static_cast<int>(_vItems.size());
}