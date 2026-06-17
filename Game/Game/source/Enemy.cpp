#include "Enemy.h"
#include "mymath.h"
#include "Player.h"
#include "DxLib.h"
#include <cstdio>

namespace
{
	static constexpr auto DAMAGE_SHAKE_STRENGTH = 50.0f;
	static constexpr auto DAMAGE_SHAKE_DURATION = 0.3f;
}


bool Enemy::Initialize()
{
	if(!base::Initialize()) { return false; }

	SetSpriteSheet(STATUS::IDLE, "res/Enemy/Enemy_Idle.png", 4, 4);
	SetSpriteSheet(STATUS::IDLE, "res/Enemy/Enemy_Idle.png", 4, 4);

	
	SetSpriteAnimTable(
	{
		{ STATUS::IDLE, {  4,  5.0f, true  } },
		{ STATUS::IDLE, {  4,  5.0f, true  } },
	});

	_spriteScale = 400.0f;

	_status = STATUS::IDLE;

	_vPos = VGet(0.0f, 50.0f, 0.0f);
	_vDir = VGet(0.0f, 0.0f, -1.0f);

	_fColSubY = 17.0f;
	_fCollisionR = 70.0f;
	_fCollisionWeight = 20.0f;

	_mvSpeed = 0.0f;
	_hp = 5.0f;

	return true;
}

bool Enemy::Terminate()
{
	base::Terminate();
	return true;
}

bool Enemy::Process()
{
	base::Process();

	STATUS oldStatus = _status;
	_status = STATUS::IDLE;

	UpdateRotation();
	UpdateSpriteAnimation(oldStatus);

	return true;
}

bool Enemy::Render()
{
	return base::Render();
}

bool Enemy::Damage(float damage)
{
	if(IsInvincible())
	{
		return false;
	}

	if(!base::Damage(damage))
	{
		return false;
	}

	_status = STATUS::DAMAGE;

	return true;
}

void Enemy::UpdateRotation()
{
	if(!_player) return;
	
	VECTOR vToPlayer = VSub(_player->GetPos(), _vPos);

	// Y軸の差を無視する
	vToPlayer.y = 0.0f;

	if(VSize(vToPlayer) > 0.0f)
	{
		_vDir = VNorm(vToPlayer);

		UpdateFacing(vToPlayer);
	}
}

