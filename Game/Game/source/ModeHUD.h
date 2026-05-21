#pragma once
#include "UIBase.h"

class ModeGame;
class Player;

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
	void UpdateHpCache();

	int _hpFullHandle { -1 };
	int _hpEmptyHandle{ -1 };
	int _hpCur        { -1 };

	// ポインター
	ModeGame* _modeGame = nullptr;
	Player* _player     = nullptr;

};