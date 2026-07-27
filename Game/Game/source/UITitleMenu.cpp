#include "UITitleMenu.h"
#include "UIRender.h"

bool UITitleMenu::Initialize()
{
	if(!base::Initialize()) { return false; }
	_selectedItem = Item::Start;
	_isDecided = false;
	_decideTimer = 0.0f;
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
		int current = static_cast<int>(_selectedItem);
		current = (current - 1 + static_cast<int>(Item::Max)) % static_cast<int>(Item::Max);
		_selectedItem = static_cast<Item>(current);
	}
	if(trg & PAD_INPUT_DOWN)
	{
		int current = static_cast<int>(_selectedItem);
		current = (current + 1) % static_cast<int>(Item::Max);
		_selectedItem = static_cast<Item>(current);
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
	int sw = ScreenW();
	int sh = ScreenH();

	// タイトルロゴ
	SetFontSize(48);
	const char* title = "MY ACTION GAME";
	int titleW = GetDrawStringWidth(title, strlen(title));
	DrawString(sw / 2 - titleW / 2, sh / 4, title, GetColor(255, 255, 255));

	// メニュー枠パネル
	int panelW = 300;
	int panelH = 120;
	int panelX = sw / 2 - panelW / 2;
	int panelY = sh / 2 + 20;
	UIRender::DrawPanel(panelX, panelY, panelW, panelH, GetColor(0, 0, 0), GetColor(255, 255, 255));

	// メニュー項目の描画
	DrawMenuItem(Item::Start, "GAME START", panelY + 20);
	DrawMenuItem(Item::Exit,  "GAME EXIT",  panelY + 65);
	return true;
}

void UITitleMenu::DrawMenuItem(Item item, const char* label, int yPos)
{
	int sw = ScreenW();
	bool isCurrent = (_selectedItem == item);

	SetFontSize(24);
	unsigned int color = GetColor(180, 180, 180);

	if(isCurrent)
	{
		if(_isDecided && (GetNowCount() / 80) % 2 == 0)
		{
			color = GetColor(255, 100, 100); // 決定時に点滅
		}
		else
		{
			color = GetColor(255, 255, 0);   // 選択中
		}
		DrawString(sw / 2 - 100, yPos, ">", color);
	}
	DrawString(sw / 2 - 70, yPos, label, color);
}