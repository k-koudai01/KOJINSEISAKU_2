#include "MenuItems.h"
#include "Title.h"

MenuItemToTitle::MenuItemToTitle()
	: MenuItemBase(nullptr, "タイトルへ戻る")
{
}

int MenuItemToTitle::Selected()
{
	if(!ModeServer::GetInstance()->Get("title"))
	{
		ModeServer::GetInstance()->Add(new Title(), 1, "title");
	}

	ModeBase* game = ModeServer::GetInstance()->Get("modeGame");
	if(game) { ModeServer::GetInstance()->Del(game); }

	ModeBase* hud = ModeServer::GetInstance()->Get("ModeHUD");
	if(hud) { ModeServer::GetInstance()->Del(hud); }

	return 1;
}