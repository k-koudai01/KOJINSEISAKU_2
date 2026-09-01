#include "PlayerNormalBullet.h"

bool PlayerNormalBullet::Initialize(const VECTOR& pos, const VECTOR& dir)
{
	if(!base::Initialize(pos, dir, 12.0f, 2.0f))
	{
		return false;
	}

	_isActive = true; 

	SetSpriteSheet(STATUS::IDLE, "res/Effect/Attack_0.png", 3, 4);
	SetSpriteAnimTable({ { STATUS::IDLE, { 3, 30.0f, true } } });

	_status		 = STATUS::IDLE;
	_radius      = 10.0f;
	_spriteScale = 80.0f;
	_damage		 = 1.0f;
	return true;
}
