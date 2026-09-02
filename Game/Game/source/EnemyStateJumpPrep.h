#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：3回ジャンプ予兆ステート
 */
class EnemyStateJumpPrep : public EnemyState
{
public:
	virtual void Enter(EnemyBase* enemy) override;
	virtual void Update(EnemyBase* enemy, float deltaTime) override;
};

