#include "Camera.h"
#include "mymath.h"
#include "ApplicationMain.h"
#include "Player.h"
#include "Enemy.h"

bool Camera::Initialize()
{
	base::Initialize();		

	_vPos      = VGet(0.0f,  0.0f, 0.0f);
	_vTarget   = VGet(0.0f, 60.0f, 0.0f);
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
	base::Process();
	BossBattleUpdate();
	UpdateShake();

	return true;
}

bool Camera::Render()
{
	/*int x = 0, y = 0, size = 16;
	SetFontSize(size);
	DrawFormatString(x, y, GetColor(255, 0, 0), "Camera:"); y += size;
	DrawFormatString(x, y, GetColor(255, 0, 0), "  target = (%5.2f, %5.2f, %5.2f)", _vTarget.x, _vTarget.y, _vTarget.z); y += size;
	DrawFormatString(x, y, GetColor(255, 0, 0), "  pos    = (%5.2f, %5.2f, %5.2f)", _vPos.x, _vPos.y, _vPos.z); y += size;
	float sx = _vPos.x - _vTarget.x;
	float sz = _vPos.z - _vTarget.z;
	float length = sqrt(sz * sz + sx * sx);
	float rad = atan2(sz, sx);
	float deg = RAD2DEG(rad);
	DrawFormatString(x, y, GetColor(255, 0, 0), "  len = %5.2f, rad = %5.2f, deg = %5.2f", length, rad, deg); y += size;*/
	return true;
}	

void Camera::MoveBy(const VECTOR& delta)
{
	_vPos    = VAdd(_vPos, delta);
	_vTarget = VAdd(_vTarget, delta);
}

void Camera::FollowUpdate()
{
	if (_player == nullptr) return;

	const VECTOR playerPos = _player->GetPos();

	// 注視点
	_vTarget = VAdd(playerPos, _targetOffset);

	// カメラ位置
	_vPos = VAdd(_vTarget, VGet(0.0f, 0.0f, -300.0f));
}

void Camera::Shake(float strength, float duration)
{
	_shakeStrength = strength;
	_shakeTimer    = duration;
	_shakeDuration = duration;
}

void Camera::BossBattleUpdate()
{
	if(_isFixedMode)
	{
		_vTarget = VGet(0.0f, 60.0f, 0.0f);

		_vPos = VAdd(_vTarget, VGet(0.0f, 0.0f, -300.0f));
	}
	else
	{
		// いつものプレイヤー追従
		FollowUpdate();
	}
}

void Camera::UpdateShake()
{
	if(_shakeTimer > 0.0f)
	{
		float decay = (_shakeDuration > 0.0f) ? (_shakeTimer / _shakeDuration) : 0.0f; 
		float currentStrength = _shakeStrength * decay; 

		float frequency = 50.0f;									    // シェイクの振動数
		float elapsedTime = _shakeDuration - _shakeTimer;			    // サイン波で上下に揺らす
		float offsetY = sin(elapsedTime * frequency) * currentStrength; // カメラのY位置にオフセットを加える

		_vPos.y = _vTarget.y + offsetY;

		_shakeTimer -= 1.0f / 60.0f;
	}
	else
	{
		_shakeTimer = 0.0f;
	}
}