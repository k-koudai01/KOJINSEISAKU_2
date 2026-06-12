#pragma once
#include "appframe.h"
#include "Player.h"
#include "Enemy.h"

class CollisionManager
{
public:
	// 更新処理
	void CheckPlayerEnemy(Player* player, Enemy* enemy);
	void CheckPlayerAttack(Player* player, Enemy* enemy);

	// 描画処理
	void DebugRenderCapsule(const Player* player, const Enemy* enemy) const;
	
};

