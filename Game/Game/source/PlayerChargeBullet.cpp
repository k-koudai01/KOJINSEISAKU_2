#include "PlayerChargeBullet.h"

bool PlayerChargeBullet::Initialize(const VECTOR& pos, const VECTOR& dir)
{
	if(!base::Initialize(pos, dir, 18.0f, 3.0f))
	{
		return false;
	}

	_isActive = true;

	SetSpriteSheet(STATUS::IDLE, "res/Effect/Attack_0.png", 3, 4);
	SetSpriteAnimTable({ { STATUS::IDLE, { 3, 30.0f, true } } });

	_status		 = STATUS::IDLE;
	_radius		 = 30.0f;
	_spriteScale = 120.0f;

	return true;
}
