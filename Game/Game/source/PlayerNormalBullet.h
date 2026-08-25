#pragma once
#include "Bullet.h"
class PlayerNormalBullet : public Bullet
{
	typedef Bullet base;
public:
	PlayerNormalBullet()		  = default;
	virtual ~PlayerNormalBullet() = default;

	bool Initialize(const VECTOR& pos, const VECTOR& dir);
};

