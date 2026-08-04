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
	bool IsAttackActive() const;
	virtual bool Damage(float damage) override;
	bool IsParryWindow() const { return (_status == STATUS::ATTACK || _status == STATUS::RUNATTACK); }
	void OnHitEnemy(); 

	// ヒットフラグの判定と設定
	bool HasHitEnemy() const { return _hasHitEnemy; }
	void SetHasHitEnemy(bool hit) { _hasHitEnemy = hit; }

	// 自動移動フラグ
	void SetAutoMove(bool enable) { _isAutoMove = enable; }

protected:
	void UpdateDamage();
	VECTOR CalculateInputVector();
	void UpdateMovement();
	void UpdateRotation();
	void UpdateJump();
	void UpdateAttack();

	//　ダメージ点滅
	bool ShouldDraw() const;

protected:
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

	bool _hasHitEnemy{ false };

	STATUS _debugOldStatus = STATUS::NONE;
protected:
};

