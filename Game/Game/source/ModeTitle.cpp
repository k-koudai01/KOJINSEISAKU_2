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

	InitTitleCamera();

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

	// スプライトアニメーションの更新
	SpriteAnimationManager::GetInstance()->Update(1.0f / 60.0f);

	UpdateTitlePlayer();

	// オブジェクト
	if(_player) { _player->Process(); }
	if(_cam)    { _cam->Process();    }

	// UI
	if(_titleUI)
	{
		_titleUI->Process();
		ProcessMenuSelection();
	}
	return true;
}

bool ModeTitle::Render()
{
	if(!_cam) return false;

	base::Render();

	RenderBackground3D();

	if(_player) { _player->Render(); }

	if(_titleUI)
	{
		SetUseZBuffer3D(FALSE);
		_titleUI->Render();
	}
	return true;
}

void ModeTitle::InitTitleCamera()
{
	if(_cam && _player)
	{
		_player->SetAutoMove(true);
		_objFtr.SetUpCamera(_cam.get(), _player.get(), false);
		CameraManager::GetInstance()->SetActiveCamera(_cam.get());
		_cam->SetTargetOffset(VGet(200.0f, 100.0f, -30.0f));
	}
}

void ModeTitle::ProcessMenuSelection()
{
	if(!_titleUI->IsDecided())return;

	switch(_titleUI->GetSelectedItem())
	{
	case UITitleMenu::Item::Start:
		ModeServer::GetInstance()->Add(new ModeGame(), 1, "modeGame");
		ModeServer::GetInstance()->Del(this);
		break;
	case UITitleMenu::Item::Exit:
		PostQuitMessage(0);
		break;
	}
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

void ModeTitle::RenderBackground3D()
{
	SetBackgroundColor(10, 15, 30);
	ClearDrawScreen();

	int sw = ApplicationBase::GetInstance()->DispSizeW();
	int sh = ApplicationBase::GetInstance()->DispSizeH();

	// 背景の描画
	if(_bgHandle != -1)
	{
		SetUseZBuffer3D(FALSE);
		DrawExtendGraph(0, 0, sw, sh, _bgHandle, TRUE);
	}

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);
	SetUseLighting(TRUE);

	if(_cam)
	{
		VECTOR pos = _cam->GetPos();
		VECTOR target = _cam->GetTarget();

		SetCameraPositionAndTarget_UpVecY(pos, target);
		SetCameraNearFar(_cam->GetClipNear(), _cam->GetClipFar());
	}
}