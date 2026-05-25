#include "ModeGame.h"
#include "ModeMenu.h"
#include "Title.h"

bool ModeGame::Initialize()
{
	if(!base::Initialize()) { return false; }

	// オブジェクト生成
	_cam    = _objFtr.CreateCamera();
	_player = _objFtr.CreatePlayer();
	_enemy  = _objFtr.CreateEnemy();

	if(!_cam || !_player || !_enemy)
	{
		return false;
	}

	// カメラ：プレイヤー追従設定
	_objFtr.SetUpCamera(_cam.get(), _player.get());

	// HUD追加
	AddHUD();

	// メニュー初期化
	_menuCtrl.Initialize();

	// ゲーム開始時刻リセット
	_gameElapsedSec = 0.0f;
	_gameClearShown = false;
	_gameOverShown = false; // 初期化
	return true;
}

bool ModeGame::Terminate()
{
	_menuCtrl.Terminate();

	_objMgr.TerminateAll();

	if(_player) {_player->Terminate(); _player.reset(); }
	if(_enemy ) { _enemy->Terminate();  _enemy.reset(); }
	if(_cam   ) {_cam->Terminate();    _cam.reset();    }
	
	base::Terminate();
	return true;
}

bool ModeGame::Process()
{
	base::Process();

	_menuCtrl.Process();

	SpriteAnimationManager::GetInstance()->Update(1.0f / 60.0f);

	if(_player) {_player->Process(); }
	if(_enemy ) { _enemy->Process(); }

	_objMgr.ProcessAll();

	if(_cam)    { _cam->Process();   }
	return true;
}

bool ModeGame::Render()
{
	if (!_cam)
	{
		return false;
	}

	// 画面を明るい色でクリア
	SetBackgroundColor(64, 64, 64);
	ClearDrawScreen();

	base::Render();

	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	//ライト設定
	SetUseLighting(TRUE);

	// カメラ設定更新
	VECTOR pos	  = _cam->GetPos();
	VECTOR target = _cam->GetTarget();


	SetCameraPositionAndTarget_UpVecY(pos, target);
	SetCameraNearFar(_cam->GetClipNear(), _cam->GetClipFar());

	// 0,0,0を中心に線を引く
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	if(_player) { _player->Render(); }
	if(_enemy ) { _enemy->Render(); }

	_objMgr.RenderAll();

	if (_cam) { _cam->Render(); }

	SetUseLighting(TRUE);
	return true;
}

void ModeGame::AddHUD()
{
	if (!ModeServer::GetInstance()->Get("ModeHUD"))
	{
		ModeServer::GetInstance()->Add(new ModeHUD(), 200, "ModeHUD");
	}
}