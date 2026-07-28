#pragma once
#include "appframe.h"
#include "ApplicationMain.h"
#include "UIBase.h"

class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	enum class Item
	{
		Retry = 0,
		Title,
		Max
	};

	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

private:
	void SelectNext();
	void SelectPrev();

	void DrawGameOverLogo();
	void DrawMenuItems();

private:
	Item _selectedItem = Item::Retry;
};