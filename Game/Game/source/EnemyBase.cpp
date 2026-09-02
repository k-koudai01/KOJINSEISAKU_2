#include "EnemyBase.h"
#include "mymath.h"
#include "Player.h"
#include "DxLib.h"
#include <cstdio>
#include "BulletManager.h"
#include "EnemyStateIdle.h"
#include "EnemyStateStun.h"
#include "EnemyStateRushAttack.h"

namespace
{
	constexpr auto DAMAGE_SHAKE_STRENGTH = 50.0f;
	constexpr auto DAMAGE_SHAKE_DURATION = 0.3f;

	constexpr float RUN_SPEED		= 8.0f; 
}


bool EnemyBase::Initialize()
{
	if(!base::Initialize()) { return false; }

	_spriteScale = 200.0f;
	_status = STATUS::IDLE;

	_vPos = VGet(0.0f, 0.0f, 0.0f);
	_baseY = _vPos.y;
	_vDir = VGet(-1.0f, 0.0f, 0.0f);

	_fColSubY = 17.0f;
	_fCollisionR = 40.0f;
	_fCollisionWeight = 20.0f;

	_mvSpeed = 0.0f;
	_hp = 5.0f;

	return true;
}

bool EnemyBase::Terminate()
{
	TerminateState();
	base::Terminate();
	return true;
}

bool EnemyBase::Process()
{
	STATUS oldStatus = _status;

	base::Process();
	UpdateInvincibleTimer();

	// 被弾無敵タイマー更新
	if(_damageTimer > 0.0f)
	{
		_damageTimer -= 1.0f / 60.0f;
	}

	// 現在のステートのAIロジックを実行
	if(_currentState)
	{
		_currentState->Update(this, 1.0f / 60.0f);
	}

	UpdateSpriteAnimation(oldStatus);
	return true;
}

bool EnemyBase::Render()
{
	if(!base::Render()) { return false; }
	return true;
}

void EnemyBase::ChangeState(EnemyState* newState)
{
	// 旧ステートの終了処理と破棄
	if(_currentState)
	{
		_currentState->Exit(this);
		delete _currentState;
		_currentState = nullptr;
	}

	_currentState = newState;

	// 新ステートの開始処理
	if(_currentState)
	{
		_currentState->Enter(this);
	}
}

bool EnemyBase::IsStunned() const
{
	return dynamic_cast<EnemyStateStun*>(_currentState) != nullptr;
}

bool EnemyBase::IsRushing() const
{
	return dynamic_cast<EnemyStateRushAttack*>(_currentState) != nullptr;
}

bool EnemyBase::Damage(float damage)
{
	if(!IsStunned() && IsInvincible())
	{
		return false;
	}

	if(!_isAlive) return false;

	if(!base::Damage(damage))
	{
		return false;
	}

	_status = STATUS::DAMAGE;
	_damageTimer = 0.5f;
	SetInvincible(0.1f);
	return true;
}

void EnemyBase::UpdateRotation()
{
	if(!_player) return;

	VECTOR vToPlayer = VSub(_player->GetPos(), _vPos);
	vToPlayer.y = 0.0f; // Y軸差分は無視

	if(VSize(vToPlayer) > 0.0f)
	{
		_vDir = VNorm(vToPlayer);
		UpdateFacing(vToPlayer);
	}
}

void EnemyBase::UpdateSpriteAnimation(STATUS oldStatus)
{
	if(_status == STATUS::DIE)
	{
		base::UpdateSpriteAnimation(oldStatus);
		return;
	}

	if(_damageTimer > 0.0f)
	{
		_status = STATUS::DAMAGE;
		base::UpdateSpriteAnimation(oldStatus);
		return;
	}

	if(_mvSpeed > 0.0f)
	{
		_status = (_mvSpeed >= 8.0f) ? STATUS::RUN : STATUS::WALK;
	}
	else
	{
		_status = STATUS::IDLE;
	}

	base::UpdateSpriteAnimation(oldStatus);
}

void EnemyBase::TerminateState()
{
	if(_currentState)
	{
		_currentState->Exit(this);
		delete _currentState;
		_currentState = nullptr;
	}
}

void EnemyBase::DebugRender()
{
	DrawFormatString(1300, 10, GetColor(255, 255, 255),
		"[Enemy Debug] Status:%d | AnimID:%d | Frame:%d",
		static_cast<int>(_status),
		_spriteAnimId,
		_frameIndex
	);
}