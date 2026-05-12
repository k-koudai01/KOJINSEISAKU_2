#pragma once
#include "appframe.h"
#include "ApplicationMain.h"

namespace UI
{
	constexpr const char* GAMEOVER_LOGO = "ゲームオーバー";
	constexpr const char* GAMEOVER_RESUME = "もう一度！";
	constexpr const char* GAMEOVER_TITLE = "おわり！";
	constexpr int GAMEOVER_SPCE = 300;
}
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

private:
	int _fontSize = 48;
};