#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：突進攻撃ステート
 * @details プレイヤーに向かって直線攻撃を行う。パリィ判定に応じてスタンまたは待機へ遷移。
 */
class EnemyStateRushAttack : public EnemyState
{
public:
	virtual void Enter(Enemy* enemy) override;
	virtual void Update(Enemy* enemy, float deltaTime) override;

	/** @brief 突進中がtrue */
	virtual bool IsRushing() const override { return true; }
};

