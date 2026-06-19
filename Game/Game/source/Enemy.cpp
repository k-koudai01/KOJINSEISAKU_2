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
	SetSpriteSheet(STATUS::WALK, "res/Enemy/Enemy_Run.png" , 8, 4);
	SetSpriteSheet(STATUS::RUN,  "res/Enemy/Enemy_Run.png" , 8, 4);

	
	SetSpriteAnimTable(
	{
		{ STATUS::IDLE, {  4,  10.0f, true  } },
		{ STATUS::WALK, {  8,  10.0f, true  } },
		{ STATUS::RUN,  {  8,  17.0f, true  } },
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
	

	UpdateAI();
	UpdateSpriteAnimation(oldStatus);

	return true;
}

bool Enemy::Render()
{
	return base::Render();
}

void Enemy::UpdateSpriteAnimation(STATUS oldStatus)
{
	// スタンを優先
	if(_bossState == BossState::STUN)
	{
		_status = STATUS::STUN;
	}

	if(_status != STATUS::RUN)
	{
		if(_mvSpeed > 0.0f)
		{
			if(_bossState == BossState::RUSH_ATTACK)
			{
				_status = STATUS::RUN;
			}
			else
			{
				_status = STATUS::WALK;
			}
		}
		else
		{
			_status = STATUS::IDLE;
		}
	}

	base::UpdateSpriteAnimation(oldStatus);
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

// 敵AI
void Enemy::UpdateAI()
{
	_stateTimer += 1.0f / 60.0f;

	VECTOR playerPos = VGet(0.0f, 0.0f, 0.0f);
	if(_player)
	{
		playerPos = _player->GetPos();
	}

	switch(_bossState)
	{
	case BossState::IDLE:
	{
		UpdateIdle(playerPos);
		break;
	}
	case BossState::RUSH_PREP:
	{
		UpdateRushPrep(playerPos);
		break;
	}
	case BossState::RUSH_ATTACK:
	{
		UpdateRushAttack();
		break;
	}
	case BossState::STUN:
	{
		UpdateStun();
		break;
	}
	}
}

void Enemy::UpdateIdle(const VECTOR& PlayerPos)
{
	_mvSpeed = 0.0f;
	UpdateRotation();

	// 指定の時間たったら突進の予兆状態へ
	if(_stateTimer >= 2.0f)
	{
		_stateTimer = 0.0f;
		_bossState = BossState::RUSH_PREP;
	}
}

void Enemy::UpdateRushPrep(const VECTOR& playerPos)
{
	_mvSpeed = 0.0f;
	UpdateRotation();

	if(_stateTimer >= 0.5f)
	{ 
		_mvSpeed = 3.0f;
		VECTOR backDir = VScale(_vDir, -1.0f);
		_vPos = VAdd(_vPos, VScale(backDir, _mvSpeed));
	}
	else
	{
		_mvSpeed = 0.0f;
	}

	if(_stateTimer >= 1.0f)
	{
		_stateTimer = 0.0f;
		_targetDir = _vDir;
		_bossState = BossState::RUSH_ATTACK;
	}
}

void Enemy::UpdateRushAttack()
{
	_mvSpeed = 8.0f;
	_vPos = VAdd(_vPos, VScale(_targetDir, _mvSpeed));

	UpdateFacing(_targetDir);

	if(_stateTimer >= 1.0f)
	{
		_stateTimer = 0.0f; 
		_bossState = BossState::STUN;
	}
}

void Enemy::UpdateStun()
{
	_mvSpeed = 0.0f;
	
	// 画像

	if(_stateTimer >= 2.0f)
	{
		_stateTimer = 0.0f; // タイマーリセット
		_bossState = BossState::IDLE;
	}
}