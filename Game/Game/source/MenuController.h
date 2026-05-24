#pragma once
#include "appframe.h"
#include "ModeMenu.h"

class MenuController
{
public:
	bool Initialize();
	bool Terminate();	
	bool Process();

private:
	bool IsEscTrg();
	bool IsMenuOpen() const;
	void OpenMenu();

	bool _prevEsc = false;
};

