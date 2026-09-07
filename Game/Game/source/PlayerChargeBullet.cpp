#include "PlayerChargeBullet.h"

namespace
{
	constexpr float BULLET_SPEED	= 10.0f; 
	constexpr float BULLET_LIFETIME = 3.0f; 
}

bool PlayerChargeBullet::Initialize(const VECTOR& pos, const VECTOR& dir)
{
	if(!base::Initialize(pos, dir, BULLET_SPEED, BULLET_LIFETIME))
	{
		return false;
	}

	_isActive = true;

	SetSpriteSheet(STATUS::IDLE, "res/Effect/Attack_0.png", 3, 4);
	SetSpriteAnimTable({ { STATUS::IDLE, { 3, 30.0f, true } } });

	_status		 = STATUS::IDLE;
	_radius		 = 30.0f;
	_fCollisionR = 30.0f;
	_spriteScale = 120.0f;
	_damage		 = 3.0f;

	return true;
}
