#pragma once
#include "EnemyState.h"

/**
 * @brief 敵AI：スタン状態ステート
 * @details パリィ成功時などの隙状態。移動や攻撃を行わず、一定時間無防備状態。
 */

class EnemyStateStun :public EnemyState
{
public:
	virtual void Enter(EnemyBase* enemy) override;
	virtual void Update(EnemyBase* enemy, float deltaTime) override;
	
	virtual bool IsStunned() const override { return true; }
};

