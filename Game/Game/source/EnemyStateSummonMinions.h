#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：手下を召喚ステート
 */
class EnemyStateSummonMinions : public EnemyState
{
public:
	virtual void Enter(Enemy* enemy) override;
	virtual void Update(Enemy* enemy, float deltaTime) override;

private:
	bool _hasSummoned = false;
};

