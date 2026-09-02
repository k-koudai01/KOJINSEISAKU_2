#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：待機ステート
 * @details 一定時間プレイヤーの方を向き、その後ランダムな攻撃行動へ遷移する。
 */
class EnemyStateIdle : public EnemyState
{
public:
	virtual void Enter(EnemyBase* enemy) override;
	virtual void Update(EnemyBase* enemy, float deltaTime) override;

};

