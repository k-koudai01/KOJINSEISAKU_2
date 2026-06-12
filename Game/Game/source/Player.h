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

	void SetCamera(Camera* cam) { _cam = cam; }

	virtual bool Damage(float damage) override;
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
	};

	int _spriteAnimId = -1;
	int _frameIndex = 0;

protected:
	void UpdateMovement();
	void UpdateRotation();
	void UpdateJump();
	void UpdateAttack();


	//　ダメージ点滅
	bool ShouldDraw() const;

	// ダメージカウンター
	float _damageCounter = 0;

	// カメラ
	Camera* _cam;

	//アナログスティック関係
	float _analogDeadZone;
	float lx, lz, rx, ry;
	DINPUT_JOYSTATE di;

	//左スティックの値
	float lStickX, lStickZ;

	// 移動方向を決める
	VECTOR _v;

	// ジャンプ用
	float _vY;
	float _gravity;
	float _jumpSpeed;
	bool  _isGrounded;

	// 攻撃関連
	bool _isAttacking{ false };

	// 無敵状態フラグ
	bool _isInvincible { false };
};

