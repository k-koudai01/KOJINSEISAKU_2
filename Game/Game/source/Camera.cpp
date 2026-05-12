#include "Camera.h"
#include "mymath.h"
#include "ApplicationMain.h"

bool Camera::Initialize()
{
	_vPos = VGet(0, 90.f, -300.f);
	_vTarget = VGet(0, 60, 0);
	_clipNear = 2.0f;
	_clipFar = 10000.0f;
	return true;
}

bool Camera::Terminate()
{
	return true;
}

bool Camera::Process(int key, int trg)
{
	// プレイヤー追従更新
	if(!_player) return true;
	//アナログスティック対応
	UseStick();

	// デッドゾーン
	const float dead = _player->analogMin;

	// カメラがターゲットを貫通しないよう最低高さを保証する
	if(_vPos.y < _vTarget.y + _minAboveTarget)
	{
		_vPos.y = _vTarget.y + _minAboveTarget;
	}
	if(_vPos.y > _vTarget.y + _maxAboveTarget)
	{
		_vPos.y = _vTarget.y + _maxAboveTarget;
	}
	return true;
}

bool Camera::Render()
{
	// カメラ設定更新
	SetCameraPositionAndTarget_UpVecY(_vPos, _vTarget);
	SetCameraNearFar(_clipNear, _clipFar);

	return true;
}	
