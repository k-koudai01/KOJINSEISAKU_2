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

	SetSpriteAnimTable
	({
		{ STATUS::IDLE,      {  4,  5.0f,  true  } },
		{ STATUS::WALK,      {  8, 10.0f,  true  } },
		{ STATUS::JUMP,      {  8, 10.0f,  true  } },
		{ STATUS::FALL,      {  8, 10.0f,  true  } },
		{ STATUS::DAMAGE,    {  5, 10.0f, false  } },
		{ STATUS::ATTACK,    {  8, 20.0f, false  } },
		{ STATUS::RUNATTACK, {  8, 20.0f, false  } },
	});

	_spriteScale = 80.0f;

	// ステータスを「無し」に設定
	_status = STATUS::IDLE;

	// 位置、向きの初期化
	_vPos = VGet(85.0f, 0.0f, 10.0f);
	_vDir = VGet(0.0f, 0.0f, -1.0f);

	// 当たり判定の初期化
	_fColSubY = 10.0f;
	_fCollisionR = 11.0f;
	_fCollisionWeight = 20.0f;

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
	lStickX = 0.0f;
	lStickZ = 0.0f;

	// ジャンプ関連の初期化
	_vY = 0.0f;
	_gravity = -0.8f;
	_jumpSpeed = 15.0f;
	_isGrounded = true;

	// 無敵状態の初期化
	_isInvincible = false;

	_damageCounter = 0;

	return true;
}

bool Player::Terminate()
{
	base::Terminate();

	return true;
}

bool Player::Process()
{
	base::Process();
	UpdateInvincibleTimer(); // 無敵時間の更新	
	STATUS oldStatus = _status;

	if(_damageCounter > 0)
	{
		_damageCounter -= 1.0f / 60.0f; 

		if(_damageCounter < 0.0f)
		{
			_status = STATUS::NONE;
		}
	}
	
	UpdateMovement();
	UpdateJump();
	UpdateRotation();
	UpdateAttack();

	UpdateFacing(_v);
	UpdateSpriteAnimation(oldStatus);

	return true;
}

bool Player::Render()
{
	int x = 0, y = 500, size = 16;
	SetFontSize(size);
	DrawFormatString(x, y, GetColor(255, 0, 0), "  pos    = (%5.2f, %5.2f, %5.2f)", _vPos.x, _vPos.y, _vPos.z); y += size;

	// DrawSphere3D(_vPos, 5.0f, 8, GetColor(255, 0, 0), GetColor(0, 255, 0), TRUE);

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

void Player::UpdateMovement()
{
	int key = ApplicationBase::GetInstance()->GetKey();
	int trg = ApplicationBase::GetInstance()->GetTrg();

	// 処理前の位置を保存
	_vOldPos = _vPos;

	// 移動方向を決める
	_v = { 0,0,0 };

	//　処理前のステータスを保存しておく
	STATUS oldStatus = _status;

	// カメラ前方向から角度を出す
	float fx = _cam->GetTarget().x - _cam->GetPos().x;
	float fz = _cam->GetTarget().z - _cam->GetPos().z;
	float camrad = atan2(fx, fz) - DEG2RAD(90.0f);

	//左スティック値
	lStickX = lx;
	lStickZ = lz;

	// ローカル入力ベクトル
	VECTOR inputLocal = VGet(0.0f, 0.0f, 0.0f);

	// 操作（キーボード）
	if(key & PAD_INPUT_UP)
	{
		inputLocal.z = -1.0f;
	}
	if(key & PAD_INPUT_DOWN)
	{
		inputLocal.z = 1.0f;
	}
	if(key & PAD_INPUT_LEFT)
	{
		inputLocal.x = -1.0f;
	}
	if(key & PAD_INPUT_RIGHT)
	{
		inputLocal.x = 1.0f;
	}

	// アナログ入力の長さ/角度
	float length = sqrt(lStickX * lStickX + lStickZ * lStickZ);
	float radStick = atan2(lStickX, lStickZ);

	// アナログ左スティック用
	if(length < _analogDeadZone)
	{
		length = 0.f;
	}
	else
	{
		length = _mvSpeed;
	}

	// アナログ入力がない場合はキーボードで移動
	if(length == 0.0f && VSize(inputLocal) > 0.0f)
	{
		length = _mvSpeed;
		radStick = atan2(inputLocal.x, inputLocal.z);
	}


	// 移動ベクトル
	_v.x = cos(radStick + camrad) * length;
	_v.z = sin(radStick + camrad) * length;

	// 優先順：アナログ > キーボード
	if(length > 0.0f)
	{
		_vInput = VGet(lStickZ, 0.0f, lStickX);
		if(VSize(_vInput) > 0.0f) _vInput = VNorm(_vInput);
	}
	else
	{
		_vInput = inputLocal;
		if(VSize(_vInput) > 0.0f) _vInput = VNorm(_vInput);
	}

	// 地上移動
	if(_isGrounded && _status != STATUS::DAMAGE && _status != STATUS::ATTACK && _status != STATUS::RUNATTACK)
	{
		if(VSize(_v) > 0.0f)
		{
			_status = STATUS::WALK;
		}
		else
		{
			_status = STATUS::IDLE;
		}
	}

	_vPos = VAdd(_vPos, _v);
}

void Player::UpdateJump()
{
	int trg = ApplicationBase::GetInstance()->GetTrg();

	// ジャンプ開始
	if(_isGrounded && (trg & PAD_INPUT_4))
	{
		_vY = _jumpSpeed;
		_isGrounded = false;
		_status = STATUS::JUMP;

		if(_status == STATUS::JUMP && !IsAnimationPlaying())
		{
			_status = STATUS::FALL;
		}
	}

	if(!_isGrounded)
	{
		_vY += _gravity;
		_vPos.y += _vY;

		// 上昇が終わったら落下へ
		if(_status == STATUS::JUMP && _vY <= 0.0f)
		{
			_status = STATUS::FALL;
		}

		// 着地判定
		if(_vPos.y <= 0.0f)
		{
			_vPos.y = 0.0f;
			_vY = 0.0f;
			_isGrounded = true;
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
	}
}

void Player::UpdateRotation()
{
	if(VSize(_v) == 0.0f) return;

	// 移動方向に向く
	_vDir = VNorm(_v);
}

bool Player::Damage(float damage)
{
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