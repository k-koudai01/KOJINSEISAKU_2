#pragma once

#include "SpriteCharaBase.h"
#include "Camera.h"
#include "Bullet.h"



class Player: public SpriteCharaBase
{	
	typedef SpriteCharaBase base;
public:

	virtual bool Initialize()override;
	virtual bool Terminate() override;
	virtual bool Process()   override;
	virtual bool Render()    override;

	// カメラ設定
	Camera* GetCamera() const   { return _cam; }
	void SetCamera(Camera* cam) { _cam = cam; }

	//　アクション関連
	bool IsAttacking() const { return _isAttacking; } 
	virtual bool Damage(float damage) override;
	bool IsParryWindow() const { return (_status == STATUS::ATTACK || _status == STATUS::RUNATTACK); }
	void OnHitEnemy(); 

	// 自動移動フラグ
	void SetAutoMove(bool enable) { _isAutoMove = enable; }

protected:

	struct SpriteAnimDef
	{
		int frames;       // フレーム数
		float fps;        // アニメーションの再生速度
		bool loop;        // ループするかどうか
	};

	std::unordered_map<STATUS, SpriteAnimDef> _spriteAnimTable =
	{
		{ STATUS::IDLE,   { 1,   1.0f, true  } }, 
		{ STATUS::WALK,   { 3,  10.0f, true  } },
		{ STATUS::JUMP,   { 1,   6.0f, false } },
		{ STATUS::FALL,   { 1,   6.0f, true  } },
		{ STATUS::DAMAGE, { 1,   6.0f, false } },
		{ STATUS::ATTACK, { 1,   6.0f, false } },
		{ STATUS::DIE,    { 1,   6.0f, false } },
	};

	int _spriteAnimId = -1;
	int _frameIndex   = 0;

protected:
	void UpdateDamage();
	VECTOR CalculateInputVector();
	void UpdateMovement();
	void UpdateRotation();
	void UpdateJump();
	void UpdateAttack();


	//　ダメージ点滅
	bool ShouldDraw() const;

	// ダメージカウンタ
	float _damageCounter = 0.0f;

	// カメラ参照
	Camera* _cam = nullptr;

	// アナログスティック関係
	float _analogDeadZone = 0.2f;
	float lx = 0.0f;
	float lz = 0.0f;

	// 移動方向ベクトル
	VECTOR _v = VGet(0.0f, 0.0f, 0.0f);

	// 自動移動
	bool _isAutoMove{ false };

	// ジャンプ関連
	float _vY = 0.0f;
	float _gravity = -0.8f;
	float _jumpSpeed = 15.0f;
	bool  _isGrounded{ true };

	// 攻撃・無敵関連
	bool _canAttack   { true  };
	bool _isInvincible{ false };
};

