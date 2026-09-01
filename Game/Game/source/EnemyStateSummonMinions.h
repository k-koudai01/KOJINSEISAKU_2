#pragma once
#include "EnemyState.h"
class EnemyStateSummonMinions : public EnemyState
{
public:
	virtual void Enter(Enemy* enemy) override;
	virtual void Update(Enemy* enemy, float deltaTime) override;

private:
	bool _hasSummoned = false;
};

