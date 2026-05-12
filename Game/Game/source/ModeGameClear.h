#pragma once
#include "appframe.h"
#include "ApplicationMain.h"

// ゲームクリア画面（モード）
// 表示中は下位レイヤーの Process を停止させます。
// 決定ボタン（PAD_INPUT_1）で閉じます。
namespace UI
{
	constexpr const char* GAMECLEAR_LOGO   = "ゲームクリア！！";
	constexpr const char* GAMECLEAR_RESUME = "もう一度！";
	constexpr const char* GAMECLEAR_TITLE  = "おわり！";
	constexpr       int   GAMECLEAR_SPCE   = 300;
}

class ModeGameClear : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

private:
};