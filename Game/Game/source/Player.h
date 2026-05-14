#pragma once

#include "CharaBase.h"
#include "Camera.h"
#include "Bullet.h"

namespace
{
	struct AnimDef
	{
		std::string name; // アニメーション名
		bool loop;        // ループするかどうか
		float speed;      // アニメーションの再生速度
	};

	const std::unordered_map<CharaBase::STATUS, AnimDef> _AnimTable = {
		{ CharaBase::STATUS::WAIT,   { "MO_SDChar_idle",         true,  31.0f } },
		{ CharaBase::STATUS::WALK,   { "MO_SDChar_run",          true,  51.0f } },
		{ CharaBase::STATUS::JUMP,   { "MO_SDChar_jumpStart",   false,  30.0f } },
		{ CharaBase::STATUS::FALL,   { "MO_SDChar_jumpLoop",     true,   1.0f } },
		{ CharaBase::STATUS::ATTACK, { "attack",                false,   1.0f } },
		
	};
}
class Player: public CharaBase
{
	typedef CharaBase base;
public:
	virtual bool Initialize()override;
	virtual bool Terminate() override;
	virtual bool Process()   override;
	virtual bool Render()    override;

	void SetCamera(Camera* cam) { _cam = cam; }

protected:

	void UpdateMovement();
	void UpdateRotation();
	void UpdateJump();
	void UpdateAnimation(STATUS oldStatus);

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
};

