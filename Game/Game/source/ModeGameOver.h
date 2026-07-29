#pragma once
#include "appframe.h"
#include "ApplicationMain.h"
#include "UIBase.h"

class ModeGameOver : public UIBase
{
	typedef UIBase base;
public:
	enum class Item
	{
		Retry = 0,
		Title,
		Max
	};

	virtual bool Initialize() override;
	virtual bool Terminate()  override;
	virtual bool Process()    override;
	virtual bool Render()	  override;

private:

	//　メニュー項目
	void SelectNext();
	void SelectPrev();

	// 描画
	void DrawGameOverLogo();
	void DrawMenuItems();

private:
	Item _selectedItem = Item::Retry;
};