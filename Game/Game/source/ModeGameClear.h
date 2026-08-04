#pragma once
#include "appframe.h"
#include "ApplicationMain.h"
#include "UIBase.h"

class ModeGameClear : public UIBase
{
	typedef UIBase base;
public:
	enum class Item
	{
		Title = 0,
		Retry,
		Max
	};

	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

private:
	// ƒƒjƒ…[€–Ú‘€ì
	void SelectNext();
	void SelectPrev();

	// •`‰æˆ—
	void DrawGameClearLogo();
	void DrawMenuItems();

private:
	Item _selectedItem = Item::Title;
	float _inputCooldown = 0.5f;
};