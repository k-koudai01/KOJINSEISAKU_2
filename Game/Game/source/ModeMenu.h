#pragma once
#include "appframe.h"

// メニュー項目用ベースクラス
class MenuItemBase
{
public:
	MenuItemBase(void* param, std::string text) 
	{
		_param = param;
		_text = text;
	}
	virtual ~MenuItemBase() 
	{

	}

	// 項目を決定したらこの関数が呼ばれる
	// return int : 0 = メニュー継続, 1 = メニュー終了
	virtual int Selected()
	{
		return 0;
	}

public:
	void* _param;
	std::string		_text;
};

// モード
class ModeMenu : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void	Add(MenuItemBase* itemBase);
	void	Clear();

protected:
	std::vector<MenuItemBase*>	_vItems;

	int _curPos     { 0 };
	int	_curAnimCnt { 0 };

private:
	void UpdateInput();
	void UpdateCursor(int trg);
	bool UpdateSelect(int trg);
	bool UpdateClose(int trg);
	void UpdateCursorAnim();

	void DrawMenuBackground(int w, int h)const;
	void DrawMenuItems(int startY)const;
	void DrawCursor(int startY)const;

	void MeasureMenuSize(int& outW, int& outH) const;
	bool HasItems() const;
	int GetItemCount() const;
};
