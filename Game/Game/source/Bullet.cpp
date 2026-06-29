#include "Bullet.h"
#include "mymath.h"	

Bullet::Bullet()
	: SpriteCharaBase()
	, _lifeTimer(0.0f)
	, _isActive(false)
{
	_vPos = VGet(0.0f, 0.0f, 0.0f);
	_vDir = VGet(0.0f, 0.0f, 0.0f);
	_speed = 0.0f;
	_radius = 10.0f;
	_spriteScale = 20.0f;
}

bool Bullet::Initialize(const VECTOR& pos, const VECTOR& dir, float speed, float maxlifeTime)
{
	if(!base::Initialize()) { return false; }

	SetSpriteSheet(STATUS::IDLE, "res/Enemy/Enemy_Idle.png", 4, 4);

	SetSpriteAnimTable
	({
		{ STATUS::IDLE, {  4,  10.0f, true  } },
	});

	_vPos      = pos;
	_speed     = speed;
	_lifeTimer = maxlifeTime;
	_isActive  = true;	
	
	_status = STATUS::IDLE;

	// •ûŒüƒxƒNƒgƒ‹‚ð³‹K‰»
	float dirLength = VSize(dir);
	if(dirLength > 0.0f)
	{
		_vDir = VNorm(dir);
	}
	else
	{
		_vDir = VGet(1.0f, 0.0f, 0.0f); 
	}

	return true;
}

bool Bullet::Terminate()
{
	base::Terminate();
	_isActive = false;
	return true;
}

bool Bullet::Process()
{
	if(!_isActive) { return false; }

	// ˆÚ“®
	_vPos = VAdd(_vPos, VScale(_vDir, _speed));

	// Žõ–½ŒvŽZ
	UpdateLifeTimer();

	STATUS oldStatus = _status;
	UpdateSpriteAnimation(oldStatus);

	return true;
}

bool Bullet::Render()
{
	if(!_isActive) return false;

	return base::Render();
}

void Bullet::UpdateLifeTimer()
{
	_lifeTimer -= 1.0f / 60.0f;
	if(_lifeTimer <= 0.0f)
	{
		_isActive = false;
	}
}