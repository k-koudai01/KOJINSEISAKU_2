#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "Player.h" 
#include "Camera.h"
#include "Gun.h"
#include "mymath.h"
#include "ModeGameClear.h"
#include "ModeGameOver.h"

bool ModeGame::Initialize()
{
	if(!base::Initialize()) { return false; }
	


	// ゲーム開始時刻リセット
	_gameElapsedSec = 0.0f;
	_gameClearShown = false;
	_gameOverShown = false; // 初期化
	return true;
}

bool ModeGame::Terminate()
{
	base::Terminate();

	return true;
}

bool ModeGame::Process()
{
	base::Process();

	

	return true;
}

bool ModeGame::Render()
{
	
	return true;
}











// プレイヤーのコリジョン処理
void ModeGame::PlayerCollision()
{
	// プレイヤーの位置を取得
	VECTOR playerPos    = _player->GetPos();
	VECTOR v            = _player->GetMoveV();
	float playercolsubY = _player->GetColSubY();
	VECTOR oldvPos		= playerPos;

	// 移動した先でコリジョン判定
	MV1_COLL_RESULT_POLY hitPoly;

	// 主人公の腰位置から下方向への直線
	hitPoly = MV1CollCheck_Line
	(_handleMap,
	 _frameMapCollision,
	  VAdd(_player->_vPos, VGet(0, _player->_colSubY, 0)),
	  VAdd(playerPos, VGet(0, -99999.f, 0)));

	if(hitPoly.HitFlag)
	{
		// 当たった
		// 当たったY位置をキャラ座標にする
		_player->_vPos.y = hitPoly.HitPosition.y;

		if(_camera)
		{
			// カメラも同じ分移動させる
			if(_cameraTP) { _cameraTP->MoveBy(v); }
			
			// FPSカメラも同じ分移動させる
			if(_fCamera && _fCamera != _cameraTP) { _fCamera->MoveBy(v); }
		}
	}
	else
	{
		// 当たらなかった。元の座標に戻す
		_player->_vPos = _player->oldPos;
	}

}
// プレイヤーと敵の当たり判定
void ModeGame::PlayerEnemyCollision(Player* pl, Enemy* en)
{
	if (!pl || !en) return;
	// 生存チェック（生きていない敵とは当たらない）
	if (!en->IsAlive()) return;

	// 円同士の当たり判定
	if (pl->IsHitCircle(pl, en))
	{
		//// ゲームオーバー用オーバーレイを一度だけ表示
		//if (!_gameOverShown)
		//{
		//	_gameOverShown = true;
		//	ModeServer::GetInstance()->Add(new ModeGameOver(), 255, "ModeGameOver");
		//}
	}
}
