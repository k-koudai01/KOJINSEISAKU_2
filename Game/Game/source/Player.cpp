#include "Player.h"
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "mymath.h"
#include "CameraManager.h"

namespace
{
	static constexpr auto DAMAGE_SHAKE_STRENGTH = 50.0f;
	static constexpr auto DAMAGE_SHAKE_DURATION = 0.3f;
}



bool Player::Initialize()
{
	// 基底クラスの初期化
	if(!base::Initialize()) { return false; }

	// スプライトシートの読み込み
	SetSpriteSheet(STATUS::IDLE,      "res/Player/Player_Idle.png"	   , 12, 4);
	SetSpriteSheet(STATUS::WALK,      "res/Player/Player_Run.png"	   ,  8, 4);
	SetSpriteSheet(STATUS::JUMP,      "res/Player/Player_Run.png"	   ,  8, 4);
	SetSpriteSheet(STATUS::FALL,      "res/Player/Player_Run.png"      ,  8, 4);
	SetSpriteSheet(STATUS::DAMAGE,    "res/Player/Player_Damage.png"   ,  5, 4);
	SetSpriteSheet(STATUS::ATTACK,    "res/Player/Player_Attack.png"   ,  8, 4);
	SetSpriteSheet(STATUS::RUNATTACK, "res/Player/Player_RunAttack.png",  8, 4);
	SetSpriteSheet(STATUS::DIE,       "res/Player/Player_Die.png"      ,  4, 4);

	SetSpriteAnimTable
	({
		{ STATUS::IDLE,      {  4,  5.0f,  true  } },
		{ STATUS::WALK,      {  8, 10.0f,  true  } },
		{ STATUS::JUMP,      {  8, 10.0f,  true  } },
		{ STATUS::FALL,      {  8, 10.0f,  true  } },
		{ STATUS::DAMAGE,    {  5, 10.0f, false  } },
		{ STATUS::ATTACK,    {  8, 20.0f, false  } },
		{ STATUS::RUNATTACK, {  8, 20.0f, false  } },
		{ STATUS::DIE,       {  4, 10.0f, false  } },
	});

	_spriteScale = 100.0f;

	// ステータスを「無し」に設定
	_status = STATUS::IDLE;

	// 位置、向きの初期化
	_vPos = VGet(-215.0f, -30.0f, 0.0f);
	_vDir = VGet(1.0f, 0.0f, 0.0f);

	// 当たり判定の初期化
	_fColSubY = 10.0f;
	_fCollisionR = 11.0f;
	_fCollisionWeight = 50.0f;

	// カメラの初期化
	_cam = nullptr;

	// 移動速度設定
	_mvSpeed = 5.0f;

	// 初期体力設定
	_hp = 5.0f;

	// アナログ初期化
	_analogDeadZone = 0.2f;
	lx = 0.0f;
	lz = 0.0f;

	// ジャンプ関連の初期化
	_vY = 0.0f;
	_gravity = -0.8f;
	_jumpSpeed = 15.0f;
	_isGrounded = true;

	// 無敵状態の初期化
	_isInvincible = false;

	_damageCounter = 0;

	_hasHitEnemy = false;

	return true;
}

bool Player::Terminate()
{
	base::Terminate();

	return true;
}

bool Player::Process()
{
	STATUS oldStatus = _status;
	_debugOldStatus = oldStatus;

	base::Process();
	UpdateInvincibleTimer(); // 無敵時間の更新	

	if(_status == STATUS::DIE)
	{
		UpdateFacing(_vDir);
		UpdateSpriteAnimation(oldStatus);
		return true;
	}

	// 各更新処理
	UpdateDamage();
	UpdateMovement();
	UpdateJump();
	UpdateRotation();
	UpdateAttack();

	// 向きとアニメションの更新
	UpdateFacing(_vDir);
	UpdateSpriteAnimation(oldStatus);

	return true;
}

bool Player::Render()
{
	/*int x = 0, y = 500, size = 16;
	SetFontSize(size);
	DrawFormatString(x, y, GetColor(255, 0, 0), "  pos    = (%5.2f, %5.2f, %5.2f)", _vPos.x, _vPos.y, _vPos.z); y += size;

	DrawFormatString(10, 1000, GetColor(255, 255, 255),
					 "PlayCount: %d | AnimID: %d | Frame: %d",
					 _playCount, _spriteAnimId, _frameIndex);*/

	if(ShouldDraw())
	{
		return base::Render();
	}

	return true;
}

bool Player::ShouldDraw() const
{
	if(IsInvincible())
	{
		int blinkInterval = 100; // 点滅の間隔
		if(GetNowCount() / blinkInterval % 2 == 0)
		{
			return false;
		}
	}
	return true;
}

void Player::UpdateDamage()
{
	if(_damageCounter > 0.0f)
	{
		_damageCounter -= 1.0f / 60.0f;

		if(_damageCounter <= 0.0f)
		{
			if(_status != STATUS::DIE)
			{
				_status = STATUS::IDLE;
			}
		}
	}
}

VECTOR Player::CalculateInputVector()
{
	int key = ApplicationBase::GetInstance()->GetKey();

	// カメラ前方方向から角度算出
	float fx = _cam->GetTarget().x - _cam->GetPos().x;
	float fz = _cam->GetTarget().z - _cam->GetPos().z;
	float camrad = atan2(fx, fz) - DEG2RAD(90.0f);

	// キーボード入力
	VECTOR inputLocal = VGet(0.0f, 0.0f, 0.0f);
	if(key & PAD_INPUT_UP   ) { inputLocal.z = -1.0f; }
	if(key & PAD_INPUT_DOWN ) { inputLocal.z =  1.0f; }
	if(key & PAD_INPUT_LEFT ) { inputLocal.x = -1.0f; }
	if(key & PAD_INPUT_RIGHT) { inputLocal.x =  1.0f; }

	// アナログスティック
	float length   = sqrt(lx * lx + lz * lz);
	float radStick = atan2(lx, lz);

	if(length < _analogDeadZone)
	{
		length = 0.0f;

	}
	else
	{
		length = _mvSpeed;
	}

	// アナログ入力がない場合はキーボードで補完
	if(length == 0.0f && VSize(inputLocal) > 0.0f)
	{
		length   = _mvSpeed;
		radStick = atan2(inputLocal.x, inputLocal.z);
	}

	// 向きの入力情報を更新
	if(length > 0.0f)
	{
		_vInput = VGet(lz, 0.0f, lx);
		if(VSize(_vInput) > 0.0f) _vInput = VNorm(_vInput);

	}
	else
	{
		_vInput = inputLocal;
		if(VSize(_vInput) > 0.0f) _vInput = VNorm(_vInput);
	}

	// カメラの向きを加味した最終移動ベクトル
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);
	moveVec.x = cos(radStick + camrad) * length;
	moveVec.z = sin(radStick + camrad) * length;

	// 移動制限
	if(!CanMoveVertical())
	{
		moveVec.z = 0.0f;
	}

	if(!CanMoveHorizontal())
	{
		moveVec.x = 0.0f;
	}

	return moveVec;
}
void Player::UpdateMovement()
{
	// 処理前の位置を保存
	_vOldPos = _vPos;
	_v = { 0, 0, 0 };

	// 自動移動時
	if(_isAutoMove)
	{
		_status = STATUS::WALK;
		_vDir = VGet(1.0f, 0.0f, 0.0f);
		_v = VGet(1.0f, 0.0f, 0.0f);
		_vPos = VAdd(_vPos, _v);
		return;
	}

	// 入力ベクトルの計算
	_v = CalculateInputVector();

	// 地上での歩き・待機ステータス切り替え
	bool canChangeGroundStatus =
		(
			_isGrounded &&
			_status != STATUS::DAMAGE &&
			_status != STATUS::ATTACK &&
			_status != STATUS::RUNATTACK
		);

	if(canChangeGroundStatus)
	{
		_status = (VSize(_v) > 0.0f) ? STATUS::WALK : STATUS::IDLE;
	}

	// 座標の更新
	_vPos = VAdd(_vPos, _v);
}

void Player::UpdateJump()
{
	if(!_canJump) return;

	int trg = ApplicationBase::GetInstance()->GetTrg();

	// ジャンプ開始
	if(_isGrounded && (trg & PAD_INPUT_4))
	{
		_vY = _jumpSpeed;
		_isGrounded = false;
		_status = STATUS::JUMP;
	}

	if(!_isGrounded)
	{
		_vY += _gravity;
		_vPos.y += _vY;

		// 上昇が切れたら落下アニメーション
		if(_status == STATUS::JUMP && _vY <= 0.0f)
		{
			_status = STATUS::FALL;
		}

		// 着地判定
		if(_vPos.y <= -30.0f)
		{
			_vPos.y = -30.0f;
			_vY     = 0.0f;
			_isGrounded = true;

			if(_status == STATUS::FALL || _status == STATUS::JUMP)
			{
				_status = STATUS::IDLE;
			}
		}
	}
}

void Player::UpdateAttack()
{
	_isAttacking = (_status == STATUS::ATTACK || _status == STATUS::RUNATTACK);

	if(_isAttacking)
	{
		// アニメーションが終わったらステータスを戻す
		if(IsSpriteAnimationFinished())
		{
			_status = STATUS::IDLE;
		}
		return; 
	}

	// 攻撃許可がないなら判定しない
	if(!_canAttack) return;

	int trg = ApplicationBase::GetInstance()->GetTrg();

	// 攻撃
	if(trg & PAD_INPUT_1)
	{
		if(VSize(_v) > 0.0f)
		{
			_status = STATUS::RUNATTACK;
		}
		else
		{
			_status = STATUS::ATTACK;
		}

		_hasHitEnemy = false;
	}
}

bool Player::IsAttackActive() const
{
	if(!_isAttacking) return false;
	return (_frameIndex >= 3 && _frameIndex <= 5);
}

void Player::UpdateRotation()
{
	if(VSize(_v) == 0.0f) return;

	// 移動方向に向く
	_vDir = VNorm(_v);
}

bool Player::Damage(float damage)
{
	if(_status == STATUS::DIE)
	{
		return false;
	}
	if(IsInvincible())
	{
		return false;
	}

	if(!base::Damage(damage))
	{
		return false;
	}

	// ダメージカウンター
	_damageCounter = 0.4f; 
	
	// カメラがセットされていればシェイクさせる
	if(_cam)
	{
		_cam->Shake(DAMAGE_SHAKE_STRENGTH, DAMAGE_SHAKE_DURATION);
	}

	_status = STATUS::DAMAGE;

	return true;
}

void Player::OnHitEnemy()
{
	if(_cam)
	{
		_cam->Shake(50.0f, 0.3f);
	}
}

void Player::SetCanControl(bool enable)
{
	SetAllowMoveHorizontal(enable);
	SetAllowMoveVertical(enable);
	SetCanJump(enable);
	_canAttack = enable;
}