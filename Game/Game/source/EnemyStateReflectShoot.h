#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：巨大ラリー弾発射ステート
 */
class EnemyStateReflectShoot : public EnemyState
{
public:
	virtual void Enter(Enemy* enemy) override;
	virtual void Update(Enemy* enemy, float deltaTime) override;

private:
	bool _hasFired = false;
};

