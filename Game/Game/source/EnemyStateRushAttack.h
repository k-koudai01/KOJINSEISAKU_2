#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：突進攻撃ステート
 */
class EnemyStateRushAttack : public EnemyState
{
public:
	virtual void Enter(Enemy* enemy) override;
	virtual void Update(Enemy* enemy, float deltaTime) override;
};

