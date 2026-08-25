#include "PlayerNormalBullet.h"

bool PlayerNormalBullet::Initialize(const VECTOR& pos, const VECTOR& dir)
{
	if(!base::Initialize(pos, dir, 12.0f, 2.0f))
	{
		return false;
	}
	SetSpriteSheet(STATUS::IDLE, "res,PlayerNormalBullet.png", 1, 1);
	SetSpriteAnimTable({ { STATUS::IDLE, { 1, 1.0f, true } } });

	_radius		 = 5.0f;
	_spriteScale = 16.0f;

	return true;
}
