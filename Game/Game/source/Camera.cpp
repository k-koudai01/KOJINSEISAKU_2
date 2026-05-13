#include "Camera.h"
#include "mymath.h"
#include "ApplicationMain.h"

bool Camera::Initialize()
{
	base::Initialize();		

	_vPos      = VGet(0.0f, 1600.0f, -662.0f);
	_vTarget   = VGet(0.0f,   60.0f,    0.0f);
	_clipNear  = 2.0f;
	_clipFar   = 10000.0f;
	_forvScale = -10.0f;
	return true;
}

bool Camera::Terminate()
{
	base::Terminate();
	return true;
}

bool Camera::Process()
{
	FollowUpdate();
	return true;
}

bool Camera::Render()
{
	// カメラ設定更新
	SetCameraPositionAndTarget_UpVecY(_vPos, _vTarget);
	SetCameraNearFar(_clipNear, _clipFar);

	return true;
}	

void Camera::MoveBy(const VECTOR& delta)
{
	_vPos    = VAdd(_vPos, delta);
	_vTarget = VAdd(_vTarget, delta);
}

void Camera::FollowUpdate()
{
	if(_player == nullptr)
	{
		return;
	}

	// Targetをプレイヤーの位置に設定
	const VECTOR playerPos = _player->GetPos();
	_vTarget = playerPos;

	// オフセット
	_vPos = VAdd(playerPos, VGet(0.0f, 0.0f, 0.0f));
}