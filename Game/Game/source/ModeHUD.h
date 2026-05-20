#pragma once
#include "UIBase.h"

class ModeGame;
class Player;

constexpr int HP_X = 80; 
constexpr int HP_Y = 60;
constexpr int HP_SPACING = 10;

class ModeHUD : public UIBase
{
	typedef UIBase base;

public:
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

private:
	// HP関連
	void DrawHp(int x, int y, int maxHp, int curHp, int spacing);

	int _hpMax        {  5   };
	int _hpFullHandle { -1   };
	int _hpEmptyHandle{ -1   };

	// ポインター
	ModeGame* _modeGame = nullptr;
	Player* _player     = nullptr;

};