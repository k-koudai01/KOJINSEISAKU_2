#include "ModeHUD.h"
#include "ModeGame.h"
#include "Player.h"
#include "UIRender.h"

namespace
{
	// HP関連
	constexpr int HP_POS_X     = 80;
	constexpr int HP_POS_Y     = 60;
	constexpr int HP_SPACING   = 10;
	constexpr int HP_MAX_COUNT = 5;

	// アイコン枠のサイズに関する定数
	constexpr float ICON_SIZE = 36.0f;  // アイコンのサイズ
	constexpr float BG_HEIGHT = 42.0f;  // 背景の高さ
	constexpr float BG_MARGIN = 6.0f;   // 背景の余白
	constexpr int   BG_ALPHA  = 160;	// 背景の透明度
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

	if(_modeGame && !_player)
	{
		_player = _modeGame->GetPlayer();
	}

	if(_player)
	{
		UpdateHpCache();
	}
	return true;
}

bool ModeHUD::Render()
{
	base::Render();

	if(!_player) { return true; }

	DrawHp(HP_POS_X, HP_POS_Y, static_cast<int>(HP_MAX_COUNT), _hpCur, HP_SPACING);
	return true;
}

void ModeHUD::DrawHp(int x, int y, int maxHp, int curHp, int spacing)
{
	// 画面サイズに合わせた拡大倍率を取得
	float scale = GetScale();

	// 基準座標やサイズにスケール掛け算
	int sx		 = static_cast<int>(x * scale);
	int sy		 = static_cast<int>(y * scale);
	int sSpacing = static_cast<int>(spacing * scale);

	// HPアイコンのサイズ
	int iconW = static_cast<int>(ICON_SIZE * scale);
	int iconH = static_cast<int>(ICON_SIZE * scale);

	// 背景のサイズ
	int bgW = maxHp * (iconW + sSpacing) - sSpacing;
	int bgH = static_cast<int>(BG_HEIGHT * scale);
	int margin = static_cast<int>(BG_MARGIN * scale); // 背景の余白

	// 背景描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BG_ALPHA);
	DrawBox
	(
		sx - margin,
		sy - margin,
		sx + bgW + margin,
		sy + bgH + margin,
		GetColor(0, 0, 0), TRUE
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// HPアイコン描画
	for(int i = 0; i < maxHp; ++i)
	{
		int handle = (i < curHp) ? _hpFullHandle : _hpEmptyHandle;
		if(handle == -1)continue;

		int drawX = sx + i * (iconW + sSpacing);
		DrawExtendGraph(drawX, sy, drawX + iconW, sy + iconH, handle, TRUE);
	}
}

void ModeHUD::UpdateHpCache()
{
	if(!_player) return;
	_hpCur = static_cast<int>(_player->GetHP());
}