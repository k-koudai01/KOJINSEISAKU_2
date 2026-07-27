#include "AppFrame.h"
#include "ModeTitle.h"
#include "ApplicationMain.h"
#include "ModeGame.h"


bool ModeTitle::Initialize()
{
	if (!base::Initialize()) return false;

	_bgHandle = LoadGraph("res/Title/origbig.png");
	// 生成
	_player = _objFtr.CreatePlayer();
	_cam	= _objFtr.CreateCamera();

	if(_cam && _player)
	{
		// プレイヤーの自動移動を有効化
		_player->SetAutoMove(true);

		// カメラの追従設定
		_objFtr.SetUpCamera(_cam.get(), _player.get(), false);
		CameraManager::GetInstance()->SetActiveCamera(_cam.get());

		// タイトル用にカメラの位置
		_cam->SetTargetOffset(VGet(200.0f, 0.0f, -30.0f));
	}

	_titleUI = std::make_unique<UITitleMenu>();
	_titleUI->Initialize();
	return true;
}

bool ModeTitle::Terminate()
{
	base::Terminate();
	return true;
}

bool ModeTitle::Process()
{
	base::Process();

	// アニメーション更新
	SpriteAnimationManager::GetInstance()->Update(1.0f / 60.0f);

	// タイトル画面のプレイヤー更新
	UpdateTitlePlayer();

	// オブジェクトの更新
	if(_player) { _player->Process(); }
	if(_cam   ) { _cam->Process();	  }

	// UIの更新
	if(_titleUI)
	{
		_titleUI->Process();

		// 決定されたらモード遷移などの判定をするだけ！
		if(_titleUI->IsDecided())
		{
			switch(_titleUI->GetSelectedItem())
			{
			case UITitleMenu::Item::Start:
				ModeServer::GetInstance()->Add(new ModeGame(), 1, "modegame");
				ModeServer::GetInstance()->Del(this);
				
				break;
			case UITitleMenu::Item::Exit:
				PostQuitMessage(0);
				break;
			}
		}
	}
	return true;
}

bool ModeTitle::Render()
{
	if(!_cam)
	{
		return false;
	}

	base::Render();

	// 3D背景のクリア・カメラ設定
	SetBackgroundColor(10, 15, 30);
	ClearDrawScreen();

	int sw = ApplicationBase::GetInstance()->DispSizeW();
	int sh = ApplicationBase::GetInstance()->DispSizeH();

	if(_bgHandle != -1)
	{
		SetUseZBuffer3D(FALSE);
		DrawExtendGraph(0, 0, sw, sh, _bgHandle, TRUE);
	}

	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);
	SetUseLighting(TRUE);

	if(_cam)
	{
		VECTOR pos    = _cam->GetPos();
		VECTOR target = _cam->GetTarget();

		SetCameraPositionAndTarget_UpVecY(pos, target);
		SetCameraNearFar(_cam->GetClipNear(), _cam->GetClipFar());
	}

	// オブジェクト描画
	if(_player) { _player->Render(); }

	if(_titleUI)
	{
		SetUseZBuffer3D(FALSE);
		_titleUI->Render();
	}
	return true;
}

void ModeTitle::UpdateTitlePlayer()
{

	if(!_player) return;

	//　位置の更新
	VECTOR pos = _player->GetPos();
	pos.x += 1.0f;

	if(pos.x > 500.0f)
	{
		pos.x = -500.0f;
	}
	_player->SetPos(pos);
}

