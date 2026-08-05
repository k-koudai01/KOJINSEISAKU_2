#pragma once
#include "appframe.h"
#include "objectbase.h"
#include "camera.h"

namespace
{
	static constexpr float INVINCIBLE_TIME_SEC = 1.5f;  // 無敵時間の最大時間
}

namespace
{
	static constexpr float ATTACK_OFFSET = 20.0f;
}

class CharaBase : public ObjectBase
{
	typedef ObjectBase base;
public:
	enum class STATUS
	{
		NONE,
		IDLE,
		WALK,
		RUN,
		JUMP,
		SECOND_JUMP,
		FALL,
		ATTACK,
		RUNATTACK,
		DASHING,
		DAMAGE,
		STUN,
		DIE,
	};
	STATUS _status;
	
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	// ステータス
	STATUS GetStatus() const      { return _status;   }
	void SetStatus(STATUS status) { _status = status; }

	//Hp
	float GetHP()  const      { return _hp;                    }
	bool IsAlive() const      { return _hp > 0.0f && _isAlive; }
	bool IsDead()  const	  { return !_isAlive;              }
	void SetAlive(bool alive) { _isAlive = alive;              }

	// 移動関連
	const VECTOR& GetInputVector() const { return _vInput; }

	// 移動速度
	float GetMoveSpeed() const  { return _mvSpeed; }

	// 当たり判定
	float GetCollisionRadius() const { return _fCollisionR; }
	VECTOR GetCollisionCenter() const 
	{
		VECTOR c = _vPos;
		c.y += _fColSubY;
		return c;
	}

	// カプセル端点
	VECTOR GetCapsuleBottom() const { return _vPos; }
	VECTOR GetCapsuleTop() const
	{
		VECTOR t = _vPos;
		t.y += _fColSubY;
		return t;
	}

	// 攻撃判定
	bool IsAttacking() const { return _isAttacking; } 
	float GetAttackRadius() const { return _attackCollisionR; }

	// 攻撃カプセルの端点
	VECTOR GetAttackCapsuleBottom() const
	{
		return VAdd(GetCapsuleBottom(), VScale(_vDir, ATTACK_OFFSET));
	}

	VECTOR GetAttackCapsuleTop() const
	{
		return VAdd(GetCapsuleTop(), VScale(_vDir, ATTACK_OFFSET));
	}

	// ダメージ
	virtual bool Damage(float damage);
	bool IsInvincible() const      { return _invincibleTimer > 0.0f; }  // 無敵状態か
	void SetInvincible(float time) { _invincibleTimer = time;        }  // 無敵状態にする

	// アニメーション関連
	int   PlayAnimation(std::string name, bool loop = false, float speed = 1.0f); // アニメーション再生。
	void  StopAnimation();									  // アニメーション停止。
	int   GetAnimId() const { return _animId; }				  // 現在のアニメーションIDを取得
	bool  IsAnimationPlaying() const;						  // アニメーションが再生中かどうかを確認
	void  ClearAnimIdIfStopped();							  // アニメーションが停止している場合、アニメーションIDをクリアする
	float GetAnimPlayTime() const { return _playTime; }       // 現在のアニメーションの再生時間を取得するゲッター
	float GetAnimTotalTime() const { return _totalTime; }     // 現在のアニメーションの総時間を取得するゲッター

	// ジャンプ関連
	void SetVY(float vy) { _vY = vy;   }
	float GetVY() const  { return _vY; }

	void SetGrounded(bool grounded) { _isGrounded = grounded; }
	bool IsGrounded() const         { return _isGrounded; }

protected:

	// 2Dビルボード用
	enum class Facing { Down = 0, Left = 1, Right = 2, Up = 3 };
	Facing _facing = Facing::Down;

	static constexpr int SpriteCols = 4;
	static constexpr int SpriteRows = 4;
	std::array<int, SpriteCols* SpriteRows> _spriteHandles{};

	// スプライト
	int _sprite{ 0 };
	const char* _spritePath = "";

	// アニメーション管理
	int _frameW { 0 };
	int _frameH { 0 };
	int _frameIndex { 0 };
	float _spriteScale { 1.0f };
	float _attachIndex { 0.0f };
	float _totalTime   { 0.0f };
	float _playTime    { 0.0f };

	//コリジョン判定
	float _fColSubY { 0.0f };         // コリジョン判定用のYオフセット
	float _fCollisionR { 0.0f };      // 円の半径
	float _fCollisionWeight { 0.0f }; // キャラの重さ

	// 体力関係
	float _hp { 0.0f };       
	bool _isAlive { true }; // 生死フラグ

	// 移動関係
	float _mvSpeed; // 移動速度
	VECTOR _vInput; // 移動ベクトル

	// 無敵時間関係
	float _invincibleTimer { 0.0f }; // 無敵時間の残り時間
	void UpdateInvincibleTimer();    // タイマー更新用関数

	// 攻撃関連
	bool  _isAttacking{ false };
	float _attackCollisionR{ 10.0f };

	// ジャンプ関連
	float _vY = 0.0f;
	float _gravity = -0.8f;
	float _jumpSpeed = 15.0f;
	bool  _isGrounded{ true };
};

