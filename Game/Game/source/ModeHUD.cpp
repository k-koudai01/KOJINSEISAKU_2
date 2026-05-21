#include "ModeHUD.h"
#include "ModeGame.h"
#include "Player.h"
#include "UIRender.h"

namespace
{
	// HP関連
	constexpr int HP_X = 80;
	constexpr int HP_Y = 60;
	constexpr int HP_SPACING = 10;
	constexpr int HP_MAX = 5;
}

bool ModeHUD::Initialize()
{
	base::Initialize();

	_hpFullHandle  = LoadGraph("res/UI/HP/HP_Full.png");
	_hpEmptyHandle = LoadGraph("res/UI/HP/HP_Empty.png");

	return true;
}

bool ModeHUD::Terminate()
{
	if(_hpFullHandle != -1)
	{
		DeleteGraph(_hpFullHandle);
		_hpFullHandle = -1;
	}
	if(_hpEmptyHandle != -1)
	{
		DeleteGraph(_hpEmptyHandle);
		_hpEmptyHandle = -1;
	}

	_player = nullptr;
	_modeGame = nullptr;

	return true;
}

bool ModeHUD::Process()
{
	base::Process();

	if(!_modeGame)
	{
		_modeGame = dynamic_cast<ModeGame*>(ModeServer::GetInstance()->Get("modeGame"));
		if(!_modeGame) { return true; }
	}
	if(!_player)
	{
		_player = _modeGame->GetPlayer();
		if(!_player){ return true; }
	}

	UpdateHpCache();
	return true;
}

bool ModeHUD::Render()
{
	base::Render();

	if(!_player) { return true; }

	DrawHp(HP_X, HP_Y, static_cast<int>(HP_MAX), _hpCur, HP_SPACING);
	return true;
}

void ModeHUD::DrawHp(int x, int y, int maxHp, int curHp, int spacing)
{
	// 背景ボックス（半透明）
	int w = maxHp * (35 + spacing) - spacing;
	int h = 42;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
	DrawBox(x - 6, y - 6, x + w + 6, y + h + 6, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for(int i = 0; i < maxHp; ++i)
	{
		// 現在のHPより小さいならフル
		int handle = (i < curHp) ? _hpFullHandle : _hpEmptyHandle;

		if(handle == -1) continue;

		DrawGraph(x + i * (32 + spacing), y, handle, TRUE);
	}
}

void ModeHUD::UpdateHpCache()
{
	if(!_player) return;
	_hpCur = static_cast<int>(_player->GetHP());
}