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
		{ CharaBase::STATUS::WAIT,   { "idle",   true,  1.0f } },
		{ CharaBase::STATUS::WALK,   { "run",    true,  1.0f } },
		{ CharaBase::STATUS::JUMP,   { "jump",   false, 1.0f } },
		{ CharaBase::STATUS::ATTACK, { "attack", false, 1.0f } },
		
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
	// カメラ
	Camera* _cam;

	//アナログスティック関係
	float _analogDeadZone; // アナログスティックの無効範囲
	float lx, lz, rx, ry;
	DINPUT_JOYSTATE di;
	//左スティックの値
	float lStickX, lStickZ;

	// 移動方向を決める
	VECTOR _v;
};

