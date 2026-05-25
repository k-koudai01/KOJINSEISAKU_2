#pragma once
#include "Player.h"
#include "Enemy.h"

class CollisionManager
{
public:
	void CheckPlayerEnemy(Player* player, Enemy* enemy);
};

