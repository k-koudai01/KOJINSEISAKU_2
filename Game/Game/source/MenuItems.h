#pragma once
#include "ModeMenu.h"

// タイトルに戻る
class MenuItemToTitle : public MenuItemBase
{
public:
	MenuItemToTitle();
	int Selected() override;
};
