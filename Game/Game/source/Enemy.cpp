#include "Enemy.h"
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


bool Enemy::Initialize()
{
	if(!base::Initialize()) { return false; }

	// スプライトシートの設定
	SetSpriteSheet(STATUS::IDLE,	"res/Enemy/Enemy_Idle.png"	, 4, 4);
	SetSpriteSheet(STATUS::WALK,	"res/Enemy/Enemy_Run.png"	, 8, 4);
	SetSpriteSheet(STATUS::RUN,		"res/Enemy/Enemy_Run.png"	, 8, 4);
	SetSpriteSheet(STATUS::DAMAGE,  "res/Enemy/Enemy_Damage.png", 6, 4);
	SetSpriteSheet(STATUS::DIE,		"res/Enemy/Enemy_Die.png"	, 8, 4);

	SetSpriteAnimTable
	({
		{ STATUS::IDLE,   { 4, 10.0f, true  } },
		{ STATUS::WALK,   { 8, 10.0f, true  } },
		{ STATUS::RUN,    { 8, 17.0f, true  } },
		{ STATUS::DAMAGE, { 6,  8.0f, false } },
		{ STATUS::DIE,    { 8, 10.0f, false } },
	});

	_spriteScale = 200.0f;
	_status = STATUS::IDLE;

	// 初期位置・コリジョン設定
	_vPos = VGet(100.0f, 0.0f, -30.0f);
	_baseY = _vPos.y;
	_vDir = VGet(-1.0f, 0.0f, 0.0f);

	_fColSubY = 17.0f;
	_fCollisionR = 40.0f;
	_fCollisionWeight = 20.0f;

	_mvSpeed = 0.0f;
	_hp = 5.0f;

	// 最初のステートを割り当て
	ChangeState(new EnemyStateIdle());
	return true;
}

bool Enemy::Terminate()
{
	TerminateState();
	base::Terminate();
	return true;
}

bool Enemy::Process()
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

bool Enemy::Render()
{
	if(!base::Render()) { return false; }
	// DebugRender();
	return true;
}

void Enemy::ChangeState(EnemyState* newState)
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

bool Enemy::IsStunned() const
{
	return dynamic_cast<EnemyStateStun*>(_currentState) != nullptr;
}

bool Enemy::IsRushing() const
{
	return dynamic_cast<EnemyStateRushAttack*>(_currentState) != nullptr;
}

bool Enemy::Damage(float damage)
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

void Enemy::UpdateRotation()
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

void Enemy::UpdateSpriteAnimation(STATUS oldStatus)
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

void Enemy::TerminateState()
{
	if(_currentState)
	{
		_currentState->Exit(this);
		delete _currentState;
		_currentState = nullptr;
	}
}

void Enemy::DebugRender()
{
	DrawFormatString(1300, 10, GetColor(255, 255, 255),
		"[Enemy Debug] Status:%d | AnimID:%d | Frame:%d",
		static_cast<int>(_status),
		_spriteAnimId,
		_frameIndex
	);
}