#pragma once
#include "appframe.h"
namespace UI
{
	constexpr const char* TITLR_LOGO = "ゴーストショット";
	constexpr const char* TITLR_HINT = "スタートするには決定ボタンを押してください";

	static constexpr int TITLR_LOGO_X = 300;
	static constexpr int TITLR_LOGO_Y = 150;

	static constexpr int TITLE_SPCE = 350;
}

class Title : public ModeBase
{
	typedef ModeBase base;        
public:
	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;

protected:
	int _selected = 0;
};

