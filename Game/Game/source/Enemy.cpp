#include "Enemy.h"
#include "mymath.h"
#include "Player.h"
#include "DxLib.h"
#include <cstdio>
#include "BulletManager.h"

namespace
{
	constexpr auto DAMAGE_SHAKE_STRENGTH = 50.0f;
	constexpr auto DAMAGE_SHAKE_DURATION = 0.3f;

	constexpr float RUN_SPEED		= 8.0f; 
	constexpr float BACK_SPEED		= 2.0f;
	constexpr float BULLET_SPEED	= 2.0f;
	constexpr float BULLET_LIFETIME = 3.0f;

	constexpr float IDLE_DURATION   = 2.0f;
	constexpr float SHOOT_FIRE_TIME = 0.2f;
	constexpr float SHOOT_END_TIME  = 0.8f;
	constexpr float RUSH_PREP_BACK  = 0.2f;
	constexpr float RUSH_PREP_END   = 0.9f;
	constexpr float RUSH_DURATION   = 1.0f;
	constexpr float STUN_DURATION   = 2.0f;
}


bool Enemy::Initialize()
{
	if(!base::Initialize()) { return false; }

	SetSpriteSheet(STATUS::IDLE,    "res/Enemy/Enemy_Idle.png"   , 4, 4);
	SetSpriteSheet(STATUS::WALK,    "res/Enemy/Enemy_Run.png"    , 8, 4);
	SetSpriteSheet(STATUS::RUN,     "res/Enemy/Enemy_Run.png"    , 8, 4);
	SetSpriteSheet(STATUS::DAMAGE,  "res/Enemy/Enemy_Damage.png" , 6, 4);
	SetSpriteSheet(STATUS::DIE,     "res/Enemy/Enemy_Die.png"    , 8, 4);

	
	SetSpriteAnimTable(
	{
		{ STATUS::IDLE,    {  4,  10.0f, true  } },
		{ STATUS::WALK,    {  8,  10.0f, true  } },
		{ STATUS::RUN,     {  8,  17.0f, true  } },
		{ STATUS::DAMAGE,  {  6,   8.0f, false } },
		{ STATUS::DIE,     {  8,  10.0f, false } },
	});

	_spriteScale = 300.0f;
	_status = STATUS::IDLE;

	_vPos = VGet(100.0f, 0.0f, 0.0f);
	_vDir = VGet(-1.0f, 0.0f, 0.0f);

	_fColSubY		  = 17.0f;
	_fCollisionR      = 70.0f;
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
	STATUS oldStatus = _status;

	base::Process();
	UpdateInvincibleTimer();

	UpdateStatusAndAI();

	UpdateSpriteAnimation(oldStatus);
	return true;
}

bool Enemy::Render()
{
	if(!base::Render()) { return false; }

	DebugRender();

	return true;
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
void Enemy::UpdateSpriteAnimation(STATUS oldStatus)
{
	// 死亡時の処理
	if(_status == STATUS::DIE)
	{
		base::UpdateSpriteAnimation(oldStatus);
		return;
	}

	// ダメージ中の処理
	if(_status == STATUS::DAMAGE)
	{
		base::UpdateSpriteAnimation(oldStatus);
		
		// ダメージアニメーションが終了したら、ステータスをIDLEに戻す
		if(_damageTimer <= 0.0f)
		{
			_status = STATUS::IDLE;
		}
		return;
	}

	if(_status != STATUS::RUN)
	{
		if(_mvSpeed > 0.0f)
		{
			_status = (_bossState == BossState::RUSH_ATTACK) ? STATUS::RUN : STATUS::WALK;
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

	// Y軸の差を無視する
	vToPlayer.y = 0.0f;

	if(VSize(vToPlayer) > 0.0f)
	{
		_vDir = VNorm(vToPlayer);

		UpdateFacing(vToPlayer);
	}
}

void Enemy::ChangeBossState(BossState newState)
{
	_bossState = newState;
	_stateTimer = 0.0f;
}

void Enemy::UpdateStatusAndAI()
{
	if(IsDead())
	{
		_status = STATUS::DIE;
		return;
	}

	if(_damageTimer > 0.0f)
	{
		_damageTimer -= 1.0f / 60.0f;
		_status = STATUS::DAMAGE;
		return;
	}

	// 生きていて被弾中でなければ AI を更新
	UpdateAI();
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
	case BossState::SHOOT_ATTACK: 
	{
		UpdateShootAttack(playerPos);
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
	if(_stateTimer >= IDLE_DURATION)
	{
		_stateTimer = 0.0f;
		ChangeBossState(BossState::SHOOT_ATTACK);
	}
}

void Enemy::UpdateShootAttack(const VECTOR& playerPos)
{
	_mvSpeed = 0.0f;
	UpdateRotation();
	
	if(_stateTimer >= SHOOT_FIRE_TIME && !_hasFired)
	{
		_hasFired = true;

		// 弾を飛ばす方向
		VECTOR bulletDir = _vDir;
		bulletDir.z = 0.0f;
		BulletManager::GetInstance()->Spawn(_vPos, bulletDir, 10.0f, 3.0f);
	}

	if(_stateTimer >= SHOOT_END_TIME)
	{
		_hasFired = false;
		ChangeBossState(BossState::RUSH_PREP);
	}

}

void Enemy::UpdateRushPrep(const VECTOR& playerPos)
{
	_mvSpeed = 0.0f;
	UpdateRotation();

	if(_stateTimer >= RUSH_PREP_BACK)
	{ 
		_mvSpeed = BACK_SPEED;
		VECTOR backDir = VScale(_vDir, -1.0f);

		backDir.z = 0.0f;
		_vPos = VAdd(_vPos, VScale(backDir, _mvSpeed));
	}
	else
	{
		_mvSpeed = 0.0f;
	}

	if(_stateTimer >= RUSH_PREP_END)
	{
		_targetDir   = _vDir;
		_targetDir.z = 0.0f;

		// 突進が始まる瞬間に、パリィフラグを false にリセットしておく
		_isParried = false;

		ChangeBossState(BossState::RUSH_ATTACK);
	}
}

void Enemy::UpdateRushAttack()
{

	_mvSpeed = RUN_SPEED;

	// 突進方向の分だけ位置を移動させる
	_targetDir.z = 0.0f;
	_vPos = VAdd(_vPos, VScale(_targetDir, _mvSpeed));

    // プレイヤーのいる方向に向く
	UpdateFacing(_targetDir);

	if(_stateTimer >= RUSH_DURATION)
	{
		ChangeBossState(_isParried ? BossState::STUN : BossState::IDLE);
	}
}

void Enemy::UpdateStun()
{
	_mvSpeed = 0.0f;
	
	if(_stateTimer >= STUN_DURATION)
	{
		ChangeBossState(BossState::IDLE);
	}
}