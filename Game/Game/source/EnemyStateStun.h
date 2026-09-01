#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：スタン（ダウン）ステート
 */
class EnemyStateStun :public EnemyState
{
public:
	virtual void Enter(Enemy* enemy) override;
	virtual void Update(Enemy* enemy, float deltaTime) override;
	virtual void Exit(Enemy* enemy) {}
};

