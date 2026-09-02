#include "Bullet.h"
#include "mymath.h"	

Bullet::Bullet()
	: base()
	, _speed(0.0f)
	, _lifeTimer(0.0f)
	, _isActive(false)
{
}

bool Bullet::Initialize(const VECTOR& pos, const VECTOR& dir, float speed, float maxLifeTime)
{
	if(!base::Initialize()) { return false; }

	// •Ï”‚Ì‰Šú‰»
	_vPos      = pos;
	_speed     = speed;
	_lifeTimer = maxLifeTime;
	_isActive  = true;

	// •ûŒüƒxƒNƒgƒ‹‚Ì³‹K‰»
	float dirLength = VSize(dir);
	_vDir = (dirLength > 0.0f) ? VNorm(dir) : VGet(1.0f, 0.0f, 0.0f);

	return true;
}

bool Bullet::Process()
{
	if(!_isActive) { return false; }
	
	STATUS oldStatus = _status;

	base::Process();

	if(UpdateHitImpact())
	{
		return true;
	}

	// ˆÚ“®
	_vPos = VAdd(_vPos, VScale(_vDir, _speed));
	
	UpdateFacing(_vDir);
	UpdateSpriteAnimation(oldStatus);

	// Žõ–½ŒvŽZ
	UpdateLifeTimer();

	return true;
}

bool Bullet::Render()
{
	if(!_isActive) return false;

	VECTOR renderPos = VAdd(_vPos, _shakeOffset);

	DrawSphere3D(_vPos, _radius, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), TRUE);

	return base::Render();
}

void Bullet::OnHitImpact(float hitStopTime, float shakeStrength)
{
	_hitStopTime   = hitStopTime;
	_shakeStrength = shakeStrength;
}

bool Bullet::UpdateHitImpact()
{
	if(_hitStopTime <= 0.0f)
	{
		_shakeOffset = VGet(0.0f, 0.0f, 0.0f);
		return false;
	}

	_hitStopTime -= 1.0f / 60.0f;

	// ƒ‰ƒ“ƒ_ƒ€‚ÉˆÊ’u‚ð×‚©‚­—h‚ç‚·
	float offsetX = MyMath::GetRandomFloat(-_shakeStrength, _shakeStrength);
	float offsetY = MyMath::GetRandomFloat(-_shakeStrength, _shakeStrength);
	float offsetZ = MyMath::GetRandomFloat(-_shakeStrength, _shakeStrength);

	_shakeOffset = VGet(offsetX, offsetY, offsetZ);

	return true;
}

void Bullet::UpdateLifeTimer()
{
	_lifeTimer -= 1.0f / 60.0f;
	if(_lifeTimer <= 0.0f)
	{
		_isActive = false;
	}
}