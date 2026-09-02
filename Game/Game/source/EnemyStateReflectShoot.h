#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：巨大ラリー弾発射ステート
 * @details プレイヤーに向けて弾を発射を行う。
 */
class EnemyStateReflectShoot : public EnemyState
{
public:
	virtual void Enter(EnemyBase* enemy) override;
	virtual void Update(EnemyBase* enemy, float deltaTime) override;

private:
	bool _hasFired = false;
};

