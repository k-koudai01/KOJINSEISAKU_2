#pragma once
#include "Bullet.h"

class PlayerChargeBullet : public Bullet
{
	typedef Bullet base;
public:

	PlayerChargeBullet()		  = default;
	virtual ~PlayerChargeBullet() = default;

	bool Initialize(const VECTOR& pos, const VECTOR& dir);
};

