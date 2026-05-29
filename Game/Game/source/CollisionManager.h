#pragma once
#include "appframe.h"
#include "Player.h"
#include "Enemy.h"

class CollisionManager
{
public:
	void CheckPlayerEnemy(Player* player, Enemy* enemy);
	void DebugRenderCapsule(const Player* player, const Enemy* enemy) const;
};

