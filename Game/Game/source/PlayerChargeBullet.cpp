#include "PlayerChargeBullet.h"

bool PlayerChargeBullet::Initialize(const VECTOR& pos, const VECTOR& dir)
{
	if(!base::Initialize(pos, dir, 18.0f, 3.0f)) { return false; }

	SetSpriteSheet(STATUS::IDLE, "res/Player/Bullet_Charge.png", 1, 1);
	SetSpriteAnimTable({ { STATUS::IDLE, { 1, 1.0f, true } } });

	_radius = 20.0f;       
	_spriteScale = 40.0f;  

	return true;
}