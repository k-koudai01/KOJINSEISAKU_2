#include "MenuController.h"
#include "MenuItems.h"

bool MenuController::Initialize()
{
	_prevEsc = false;
	return true;
}

bool MenuController::Terminate()
{
	return true;
}

bool MenuController::Process()
{
	if(!IsEscTrg())
	{
		return true;
	}

	if (IsMenuOpen())
	{
		return true;
	}

	OpenMenu();
	return true;
}

bool MenuController::IsEscTrg()
{
	const bool esc = CheckHitKey(KEY_INPUT_ESCAPE) != 0;
	const bool escTrg = esc && !_prevEsc;
	_prevEsc = esc;
	return escTrg;
}

bool MenuController::IsMenuOpen() const
{
	return ModeServer::GetInstance()->Get("ModeMenu") != nullptr;
}

void MenuController::OpenMenu()
{
	auto* menu = new ModeMenu();
	menu->Add(new MenuItemToTitle());

	ModeServer::GetInstance()->Add(menu, 300, "ModeMenu");
}